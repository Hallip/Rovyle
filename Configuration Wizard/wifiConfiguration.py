import re
import subprocess

def getsidds():
    results = subprocess.check_output(["netsh","wlan","show","Network"])
    results = str(results).replace("\\r","")
    results = results.split("\\n")

    results = results[4:]

    sidds = []

    for x in results:
        if ":" in x and not "    " in x:
            name = re.search("[^:]*", x)
            if len(x.replace(name[0],"")[2:]):
                sidds.append(x.replace(name[0],"")[2:])
    return(sidds)