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

#ifndef INSTANCEIMPL_H_
#define INSTANCEIMPL_H_

#include <Instance.h>
#include <SessionBase.h>
#include <Channel.h>
#include <ChannelProxy.h>
#include <JsonXValue.h>

#include <memory>

namespace FreeAX25_Zero {

class InstanceImpl: public FreeAX25::Runtime::SessionBase {
public:
	/**
	 * Default constructor, needed for dictionary support.
	 */
	InstanceImpl();

	/**
	 * Constructor.
	 * @param instance_descriptor Instance descriptor
	 */
	InstanceImpl(const FreeAX25::Runtime::Instance& instance_descriptor);

	/**
	 * Destructor.
	 */
	~InstanceImpl();

	/**
	 * Initialize this instance.
	 */
	void init();

	/**
	 * Start this instance.
	 */
	void start();

	/**
	 * Get the echo parameter.
	 * @return Echo parameter.
	 */
	bool echo() const { return m_echo; }

private:
	FreeAX25::Runtime::ChannelProxy onDownlinkConnect(
			FreeAX25::Runtime::ChannelProxy target,
			std::unique_ptr<JsonX::Object>&& parameter);

	const FreeAX25::Runtime::Instance& m_instance_descriptor;
	FreeAX25::Runtime::Channel         m_downlink;
	bool                               m_echo{false};
};

} /* namespace FreeAX25_Zero */

#endif /* INSTANCEIMPL_H_ */
