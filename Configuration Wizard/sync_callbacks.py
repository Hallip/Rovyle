from __future__ import print_function	# For Py2/3 compatibility
import eel, random
import wifiConfiguration

eel.init('web')

@eel.expose
def wifiList():
    return wifiConfiguration.getsidds()

@eel.expose
def wifiConfig():
    eel.start('wifiConf.html', block=True, size=(1280, 720), port=18081)
    return


eel.start('sync_callbacks.html', block=False, size=(1280, 720), port=18080)

# Synchronous calls must happen after start() is called

# Get result returned synchronously by 
# passing nothing in second brackets
#                   v
#n = eel.wifiConfig()()
#print('Got this from Javascript:', n)


while True:
    eel.sleep(0.1)