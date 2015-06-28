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

#ifndef SESSIONIMPL_H_
#define SESSIONIMPL_H_

#include <Instance.h>
#include <SessionBase.h>
#include <Channel.h>
#include <ChannelProxy.h>
#include <JsonXValue.h>

#include <mutex>

namespace FreeAX25_Zero {

class InstanceImpl;

class SessionImpl: public FreeAX25::Runtime::SessionBase {
	friend class InstanceImpl;

public:
	/**
	 * Default constructor. Needed for dictionary support.
	 */
	SessionImpl();

	/**
	 * Constructor
	 * @param instance_impl The instance this session belongs to.
	 */
	SessionImpl(const InstanceImpl& instance_impl);

	/**
	 * Destructor.
	 */
	~SessionImpl();

private:
	void _exit() noexcept;

	FreeAX25::Runtime::ChannelProxy onDownlinkConnect(
			FreeAX25::Runtime::ChannelProxy target,
			std::unique_ptr<JsonX::Object>&& parameter);
	void onDownlinkOpen(std::unique_ptr<JsonX::Object>&&);
	void onDownlinkClose(std::unique_ptr<JsonX::Object>&&);
	void onDownlinkReceive(std::unique_ptr<JsonX::Object>&&,
			FreeAX25::Runtime::MessagePriority);
	std::unique_ptr<JsonX::Object> onDownlinkCtrl(std::unique_ptr<JsonX::Object>&&);

	const InstanceImpl&        m_instance;
	FreeAX25::Runtime::Channel m_downlink;
	bool                       m_exiting{false};
	std::mutex                 m_mutex{};
};

} /* namespace FreeAX25_Zero */
#endif /* SESSIONIMPL_H_ */

