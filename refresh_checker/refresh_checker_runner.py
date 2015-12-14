#!/usr/bin/python
import time
import os

while 1:
    print "%s" % time.ctime()
    os.system('./refresh_checker')
    time.sleep( 60 )
