/*
 Project FreeAX25 Zero
 Copyright (C) 2015  tania@df9ry.de

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Environment.h>
#include <ServerEndPoint.h>

#include "SessionImpl.h"
#include "InstanceImpl.h"

using namespace std;
using namespace FreeAX25::Runtime;

namespace FreeAX25_Zero {

SessionImpl::SessionImpl():
		m_instance{},
		m_downlink{ SessionBase::m_pointer }
{
}

SessionImpl::SessionImpl(const InstanceImpl& instance_impl):
		m_instance{ instance_impl },
		m_downlink{ SessionBase::m_pointer }
{
	env().logInfo(
			"New FreeAX25_Zero::SessionImpl(" + m_id + ")");
	m_downlink.connectFunction = [this](ChannelProxy target, unique_ptr<JsonX::Object>&& parameter)
			{ return onDownlinkConnect(target, move(parameter)); };
}

SessionImpl::~SessionImpl() {
	env().logInfo(
			"Del FreeAX25_Zero::SessionImpl(" + m_id + ")");
}

ChannelProxy SessionImpl::onDownlinkConnect(ChannelProxy target, unique_ptr<JsonX::Object>&& parameter) {
	env().logInfo(
			"FreeAX25_Zero::onDownlinkConnect(" + m_id + " from " +
				target.id() + ", " + parameter.get()->toJsonString() +
			")");
	// Disconnect connect function:
	m_downlink.connectFunction = nullptr;
	// Connect other functions:
	m_downlink.openFunction = [this](std::unique_ptr<JsonX::Object>&& parameter)
		{ onDownlinkOpen(move(parameter)); };
	m_downlink.closeFunction = [this](std::unique_ptr<JsonX::Object>&& parameter)
		{ onDownlinkClose(move(parameter)); };
	m_downlink.ctrlFunction = [this](unique_ptr<JsonX::Object>&& parameter)
			{ return onDownlinkCtrl(move(parameter)); };
	// Set downlink target:
	setRemote(m_downlink, target);
	return m_downlink.getLocalProxy();
}

void SessionImpl::onDownlinkOpen(std::unique_ptr<JsonX::Object>&& parameter) {
	env().logInfo(
			"FreeAX25_Zero::onDownlinkOpen(" + m_id + ", " +
				parameter.get()->toJsonString() +
			")");
	m_downlink.openFunction = nullptr;
	m_downlink.receiveFunction = [this](std::unique_ptr<JsonX::Object>&& parameter,
			FreeAX25::Runtime::MessagePriority priority)
		{ onDownlinkReceive(move(parameter), priority); };
}

void SessionImpl::onDownlinkClose(std::unique_ptr<JsonX::Object>&& parameter) {
	env().logInfo(
			"FreeAX25_Zero::onDownlinkClose(" + m_id + ", " +
				parameter.get()->toJsonString() +
			")");
	_exit();
}

void SessionImpl::onDownlinkReceive(std::unique_ptr<JsonX::Object>&& message,
		FreeAX25::Runtime::MessagePriority priority)
{
	env().logInfo(
			"FreeAX25_Zero::onDownlinkReceive(" + m_id + ", " +
				message.get()->toJsonString() + ", " +
				((priority == MessagePriority::PRIORITY) ? "PRIORITY" : "ROUTINE" ) +
			")");
	if (m_instance.echo())
		m_downlink.send(move(message), priority);
}

std::unique_ptr<JsonX::Object> SessionImpl::onDownlinkCtrl(std::unique_ptr<JsonX::Object>&& request) {
	env().logInfo(
			"FreeAX25_Zero::onDownlinkCtrl(" + m_id + ", " +
				request.get()->toJsonString() + ", " +
			")");
	if (m_instance.echo()) {
		return move(request);
	} else {
		return JsonX::Object::make();
	}
}

void SessionImpl::_exit() noexcept {
	{
		lock_guard<mutex> lock(m_mutex);
		if (m_exiting) return;
		m_exiting = true;
		m_downlink.connectFunction = nullptr;
		m_downlink.openFunction = nullptr;
		m_downlink.receiveFunction = nullptr;
		m_downlink.closeFunction = nullptr;
		m_downlink.ctrlFunction = nullptr;
	}
	try { m_downlink.close(); } catch(...) {}
	try { m_downlink.reset(); } catch(...) {}
	try { reset(); } catch(...) {}
}

} /* namespace FreeAX25_Zero */
