#!/bin/bash
gst-launch-1.0 udpsrc port=5600 ! udpsink host=172.16.116.150 port=5601
