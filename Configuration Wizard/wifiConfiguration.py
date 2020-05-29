import re
import subprocess

def getsidds():
    results = subprocess.check_output(["netsh","wlan","show","Network",  "mode=Bssid"])
    results = str(results).replace("\\r","")
    results = results.split("\\n")
    results = results[4:]

    sidds = []
    intencities = []
    for x in results:
        print(x)
        if ":" in x and not "    " in x:
            name = re.search("[^:]*", x)
            if len(x.replace(name[0],"")[2:]):
                sidds.append(x.replace(name[0],"")[2:])
        elif "Signal" in x:
            name = re.search("[^:]*", x)
            if len(x.replace(name[0],"")[2:]):
                intencities.append(x.replace(name[0],"")[2:])
    nets = []
    for n in range(0,len(sidds)):
        data = {
            "name": sidds[n],
            "intencity": intencities[n]
        }
        nets.append(data)
    return(nets)