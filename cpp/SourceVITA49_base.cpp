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

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

 	Source: SourceVITA49_base.cpp
 	Generated on: Fri Jan 10 11:20:07 EST 2014
 	REDHAWK IDE
 	Version: 1.8.6
 	Build id: N201312130030

*******************************************************************************************/

#include "SourceVITA49_base.h"

/******************************************************************************************

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/
 
SourceVITA49_base::SourceVITA49_base(const char *uuid, const char *label) :
                                     Resource_impl(uuid, label), serviceThread(0) {
    construct();
}

void SourceVITA49_base::construct()
{
    Resource_impl::_started = false;
    loadProperties();
    serviceThread = 0;
    
    PortableServer::ObjectId_var oid;
    dataVITA49_in = new BULKIO_dataVITA49_In_i("dataVITA49_in", this);
    oid = ossie::corba::RootPOA()->activate_object(dataVITA49_in);
    dataChar_out = new BULKIO_dataChar_Out_i("dataChar_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataChar_out);
    dataOctet_out = new BULKIO_dataOctet_Out_i("dataOctet_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataOctet_out);
    dataShort_out = new BULKIO_dataShort_Out_i("dataShort_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataShort_out);
    dataUshort_out = new BULKIO_dataUshort_Out_i("dataUshort_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataUshort_out);
    dataFloat_out = new BULKIO_dataFloat_Out_i("dataFloat_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataFloat_out);
    dataDouble_out = new BULKIO_dataDouble_Out_i("dataDouble_out", this);
    oid = ossie::corba::RootPOA()->activate_object(dataDouble_out);

    registerInPort(dataVITA49_in);
    registerOutPort(dataChar_out, dataChar_out->_this());
    registerOutPort(dataOctet_out, dataOctet_out->_this());
    registerOutPort(dataShort_out, dataShort_out->_this());
    registerOutPort(dataUshort_out, dataUshort_out->_this());
    registerOutPort(dataFloat_out, dataFloat_out->_this());
    registerOutPort(dataDouble_out, dataDouble_out->_this());
}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void SourceVITA49_base::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
}

void SourceVITA49_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    if (serviceThread == 0) {
        serviceThread = new ProcessThread<SourceVITA49_base>(this, 0.1);
        serviceThread->start();
    }
    
    if (!Resource_impl::started()) {
    	Resource_impl::start();
    }
}

void SourceVITA49_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    // release the child thread (if it exists)
    if (serviceThread != 0) {
        if (!serviceThread->release(2)) {
            throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
        }
        serviceThread = 0;
    }
    
    if (Resource_impl::started()) {
    	Resource_impl::stop();
    }
}

CORBA::Object_ptr SourceVITA49_base::getPort(const char* _id) throw (CORBA::SystemException, CF::PortSupplier::UnknownPort)
{

    std::map<std::string, Port_Provides_base_impl *>::iterator p_in = inPorts.find(std::string(_id));
    if (p_in != inPorts.end()) {

        if (!strcmp(_id,"dataVITA49_in")) {
            BULKIO_dataVITA49_In_i *ptr = dynamic_cast<BULKIO_dataVITA49_In_i *>(p_in->second);
            if (ptr) {
                return BULKIO::dataVITA49::_duplicate(ptr->_this());
            }
        }
    }

    std::map<std::string, CF::Port_var>::iterator p_out = outPorts_var.find(std::string(_id));
    if (p_out != outPorts_var.end()) {
        return CF::Port::_duplicate(p_out->second);
    }

    throw (CF::PortSupplier::UnknownPort());
}

void SourceVITA49_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the component running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    // deactivate ports
    releaseInPorts();
    releaseOutPorts();

    delete(dataVITA49_in);
    delete(dataChar_out);
    delete(dataOctet_out);
    delete(dataShort_out);
    delete(dataUshort_out);
    delete(dataFloat_out);
    delete(dataDouble_out);
 
    Resource_impl::releaseObject();
}

void SourceVITA49_base::loadProperties()
{
    addProperty(interface,
                "eth1", 
               "interface",
               "",
               "readwrite",
               "",
               "external",
               "configure");

    addProperty(attachment_override,
               "attachment_override",
               "",
               "readwrite",
               "",
               "external",
               "configure");

    addProperty(connection_status,
               "connection_status",
               "",
               "readonly",
               "",
               "external",
               "configure");

    addProperty(VITA49Processing_override,
               "VITA49Processing_override",
               "",
               "readwrite",
               "",
               "external",
               "configure");

    addProperty(advanced_configuration,
               "advanced_configuration",
               "",
               "readwrite",
               "",
               "external",
               "configure");

}
