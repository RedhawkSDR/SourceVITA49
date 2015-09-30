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

DEBUG_LEVEL = 3

import unittest
import ossie.utils.testing
import os
from omniORB import any
from ossie.utils import sb

# Full functionality is tested via end-to-end testing using SinkVITA49 and SourceVITA49 in the fulltest_VITA49.py file
class ResourceTests(ossie.utils.testing.ScaComponentTestCase):
    """Test for all resource implementations in rh.SourceVITA49"""


    def setUp(self):
        """Set up the unit test - this is run before every method that starts with test
        """
        ossie.utils.testing.ScaComponentTestCase.setUp(self)
        
        self.componentSetup()
        
        
    def tearDown(self):
        """Finish the unit test - this is run after every method that starts with test
        """
        self.comp.stop()
        
        #######################################################################
        # Simulate regular component shutdown
        self.comp.releaseObject()
           
        ossie.utils.testing.ScaComponentTestCase.tearDown(self)


    def componentSetup(self):
        """Standard start-up for testing the component
        """
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will warn about using a built-in table'
        print ' * for leap seconds if no leap seconds file is found at the indicated location.'
        #######################################################################
        # Launch the component with the default execparams
        execparams = self.getPropertySet(kinds=("execparam",), modes=("readwrite", "writeonly"), includeNil=False)
        execparams = dict([(x.id, any.from_any(x.value)) for x in execparams])
        execparams["DEBUG_LEVEL"] = DEBUG_LEVEL
        self.launch(execparams, initialize=True)
        
        #######################################################################
        # Simulate regular component startup
        configureProps = self.getPropertySet(kinds=("configure",), modes=("readwrite", "writeonly"), includeNil=False)
        self.comp.configure(configureProps)
        
        # SourceVITA49 needs to be configured prior to being started, so it must be started in each unit test.
        #self.comp.start()
        
        
    def testStartStopRelease(self):
        """testStartStopRelease - Only start, stop, then release the component.
        """
        # All but start() is accomplished by the setUp and tearDown functions called for each test.
    
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will be unable to determine attachment'
        print ' * settings using the default configuration, and will fail to launch RX thread.'
        self.comp.start()
        #self.comp.stop()

        #######################################################################
        # Simulate regular resource shutdown
        #self.comp.releaseObject()


    def testScaBasicBehavior(self):
        """testScaBasicBehavior
        """
        #######################################################################
        # Verify the basic state of the resource
        self.assertNotEqual(self.comp, None)
        self.assertEqual(self.comp.ref._non_existent(), False)
        self.assertEqual(self.comp.ref._is_a("IDL:CF/Resource:1.0"), True)

        #######################################################################
        # Validate that query returns all expected parameters
        # Query of '[]' should return the following set of properties
        expectedProps = []
        expectedProps.extend(self.getPropertySet(kinds=("configure", "execparam"), modes=("readwrite", "readonly"), includeNil=True))
        expectedProps.extend(self.getPropertySet(kinds=("allocate",), action="external", includeNil=True))
        props = self.comp.query([])
        props = dict((x.id, any.from_any(x.value)) for x in props)
        # Query may return more than expected, but not less
        for expectedProp in expectedProps:
            self.assertEquals(props.has_key(expectedProp.id), True)

        #######################################################################
        # Verify that all expected ports are available
        for port in self.scd.get_componentfeatures().get_ports().get_uses():
            port_obj = self.comp.getPort(str(port.get_usesname()))
            self.assertNotEqual(port_obj, None)
            self.assertEqual(port_obj._non_existent(), False)
            self.assertEqual(port_obj._is_a("IDL:CF/Port:1.0"),  True)

        for port in self.scd.get_componentfeatures().get_ports().get_provides():
            port_obj = self.comp.getPort(str(port.get_providesname()))
            self.assertNotEqual(port_obj, None)
            self.assertEqual(port_obj._non_existent(), False)
            self.assertEqual(port_obj._is_a(port.get_repid()),  True)
        

    def testPortConnections(self):
        """testPortConnections
        """
        #######################################################################
        # Connect to the ports
        sink = sb.DataSink();
        self.comp.connect(sink,providesPortName='charIn')
        self.comp.connect(sink,providesPortName='octetIn')
        self.comp.connect(sink,providesPortName='shortIn')
        self.comp.connect(sink,providesPortName='ushortIn')
        self.comp.connect(sink,providesPortName='floatIn')
        self.comp.connect(sink,providesPortName='doubleIn')
        
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will be unable to determine attachment'
        print ' * settings using the default configuration, and will fail to launch RX thread.'

        self.comp.start()


    def testPropertyConfiguration(self):
        """testPropertyConfiguration
        """
        #######################################################################
        # Attempt to set all the properties
        self.comp.interface = "eth0" 
        self.comp.attachment_override.ip_address = "127.0.0.1"
        self.comp.attachment_override.vlan = 0
        self.comp.attachment_override.use_udp_protocol = True
        self.comp.attachment_override.port = 12345
        self.comp.attachment_override.enabled = True 
        self.comp.VITA49Processing_override.VRL_frames = True
        self.comp.VITA49Processing_override.repeating = 1
        self.comp.VITA49Processing_override.event_tag_size = 0
        self.comp.VITA49Processing_override.channel_tag_size = 0
        self.comp.VITA49Processing_override.data_item_format = 5
        self.comp.VITA49Processing_override.vector_size = 1
        self.comp.VITA49Processing_override.processing_efficient = True
        self.comp.VITA49Processing_override.real_complex_type = 1
        self.comp.VITA49Processing_override.enable = True
        self.comp.advanced_configuration.vita49_packet_size = 1500
        self.comp.advanced_configuration.buffer_size = 4096000
        self.comp.advanced_configuration.poll_in_time = 1
        self.comp.advanced_configuration.corba_transfersize = 1023
        #self.comp.api() 
        
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will fail to connect to unicast'
        print ' * udp socket and will fail to launch the RX thread.'
        self.comp.start()

if __name__ == "__main__":
    ossie.utils.testing.main("../SourceVITA49.spd.xml") # By default tests all implementations
