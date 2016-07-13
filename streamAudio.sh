#!/bin/bash

gst-launch-1.0 alsasrc device="hw:1,0" ! queue ! audioresample ! audioconvert !  mulawenc ! rtppcmupay ! udpsink host=132.66.199.244 port=5600 sync=false &
