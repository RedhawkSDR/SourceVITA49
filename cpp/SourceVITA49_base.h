/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.

 * This is the component code. This file contains the child class where
 * custom functionality can be added to the component. Custom
 * functionality to the base class can be extended here. Access to
 * the ports can also be done from this class
 */

#ifndef SOURCEVITA49IMPLBASE_H
#define SOURCEVITA49IMPLBASE_H

#include <boost/thread.hpp>
#include <ossie/Resource_impl.h>
#include <ossie/ThreadedComponent.h>

#include <bulkio/bulkio.h>
#include "struct_props.h"

class SourceVITA49_base : public Resource_impl, protected ThreadedComponent
{
    public:
        SourceVITA49_base(const char *uuid, const char *label);
        ~SourceVITA49_base();

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void loadProperties();

    protected:
        // Member variables exposed as properties
        std::string interface;
        attachment_override_struct attachment_override;
        connection_status_struct connection_status;
        VITA49Processing_override_struct VITA49Processing_override;
        advanced_configuration_struct advanced_configuration;

        // Ports
        bulkio::InVITA49Port *dataVITA49_in;
        bulkio::OutCharPort *dataChar_out;
        bulkio::OutOctetPort *dataOctet_out;
        bulkio::OutShortPort *dataShort_out;
        bulkio::OutUShortPort *dataUshort_out;
        bulkio::OutFloatPort *dataFloat_out;
        bulkio::OutDoublePort *dataDouble_out;

    private:
};
#endif // SOURCEVITA49_IMPL_BASE_H
