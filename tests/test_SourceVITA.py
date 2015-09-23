#!/usr/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
import unittest
import ossie.utils.testing
import os
from omniORB import any
from ossie.utils.bulkio import bulkio_data_helpers
import sys
import time
import threading
from new import classobj
from omniORB import any, CORBA
from bulkio.bulkioInterfaces import BULKIO, BULKIO__POA
from ossie.cf import CF, CF__POA
from ossie.utils import uuid
from ossie.properties import simple_property
import subprocess
from ossie.utils import sb
import struct

class ComponentTests(ossie.utils.testing.ScaComponentTestCase):
    def testFunctionality(self):
        #full functionality can be tested using the SinkVITA49 and the test_VITA49.py file
        print "----------------------------- testFunctionality-------------------------"
        source = sb.launch('../SourceVITA49.spd.xml',execparams={"DEBUG_LEVEL":0})
        outputS=sb.DataSink();
        passed = True;
        #try to connect to the ports
        try:
            source.connect(outputS,providesPortName='charIn')
            source.connect(outputS,providesPortName='octetIn')
            source.connect(outputS,providesPortName='shortIn')
            source.connect(outputS,providesPortName='ushortIn')
            source.connect(outputS,providesPortName='floatIn')
            source.connect(outputS,providesPortName='doubleIn')
        except:
            print "Failed to connection to output port"
            passed = False
            #test basic functionality

        try:
            #attempt to set all the properties
            source.interface = "eth0" 
            source.attachment_override.ip_address = "127.0.0.1"
            source.attachment_override.vlan = 0
            source.attachment_override.use_udp_protocol = True
            source.attachment_override.port = 12345
            source.attachment_override.enabled = True 
            source.VITA49Processing_override.VRL_frames = True
            source.VITA49Processing_override.repeating = 1
            source.VITA49Processing_override.event_tag_size = 0
            source.VITA49Processing_override.channel_tag_size = 0
            source.VITA49Processing_override.data_item_format = 5
            source.VITA49Processing_override.vector_size = 1
            source.VITA49Processing_override.processing_efficient = True
            source.VITA49Processing_override.real_complex_type = 1
            source.VITA49Processing_override.enable = True
            source.advanced_configuration.vita49_packet_size = 1500
            source.advanced_configuration.buffer_size = 4096000
            source.advanced_configuration.poll_in_time = 1
            source.advanced_configuration.corba_transfersize = 1023
            #source.api() 
        except:
            print "\nUnit Test  ....................",u'\u2716'
            passed = False
        
        #attempt to start the component
        try:
            sb.start()
        #attempt to stop the component
            sb.stop()
        except:
             print "\nUnit Test  ....................",u'\u2716'
             passed = False
        if passed:
            print "\nUnit Test  ....................",u'\u2714'
        else:
            print "\nUnit Test  ....................",u'\u2716'
            
if __name__ == "__main__":
    ossie.utils.testing.main("../SourceVITA49.spd.xml") # By default tests all implementations

