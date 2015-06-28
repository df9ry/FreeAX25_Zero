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

#include "InstanceImpl.h"
#include "SessionImpl.h"

#include <Environment.h>
#include <ServerEndPoint.h>

using namespace std;
using namespace FreeAX25::Runtime;

namespace FreeAX25_Zero {

InstanceImpl::InstanceImpl():
			m_instance_descriptor{},
			m_downlink{ SessionBase::m_pointer }
{
}

InstanceImpl::InstanceImpl(const Instance& instance_descriptor):
			m_instance_descriptor{ instance_descriptor },
			m_downlink{ SessionBase::m_pointer }
{
	env().logInfo(
			"New FreeAX25_Zero::InstanceImpl(" + m_instance_descriptor.getName() + ")");
}

InstanceImpl::~InstanceImpl() {
	env().logInfo(
			"Del FreeAX25_Zero::InstanceImpl(" + m_instance_descriptor.getName() + ")");
}

void InstanceImpl::init() {
	env().logInfo(
			"Init FreeAX25_Zero::InstanceImpl(" + m_instance_descriptor.getName() + ")");
	m_echo = Setting::asBoolValue(m_instance_descriptor.settings, "echo");
	for (UniquePointerDictConstIterator<ServerEndPoint> iter = m_instance_descriptor.serverEndPoints.begin();
			iter != m_instance_descriptor.serverEndPoints.end(); ++iter)
	{
		const ServerEndPoint& sep{*iter->second.get()};
		env().logInfo("Publish endpoint \"" + sep.getName() + "\" as \"" + sep.getUrl() + "\"");
		env().serverProxies.insertCopy(sep.getUrl(), m_downlink.getLocalProxy());
	}
}

void InstanceImpl::start() {
	env().logInfo(
			"Start FreeAX25_Zero::InstanceImpl(" + m_instance_descriptor.getName() + ")");
	m_downlink.connectFunction = [this](ChannelProxy target, unique_ptr<JsonX::Object>&& parameter)
			{ return onDownlinkConnect(target, move(parameter)); };
}

ChannelProxy InstanceImpl::onDownlinkConnect(
		FreeAX25::Runtime::ChannelProxy target,
		std::unique_ptr<JsonX::Object>&& parameter)
{
	SessionImpl* ps = new SessionImpl(*this);
	return ps->onDownlinkConnect(target, move(parameter));
}


} /* namespace FreeAX25_Zero */
