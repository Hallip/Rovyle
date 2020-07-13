import json
from elasticsearch import Elasticsearch
import datetime
import time
import urllib3

es = Elasticsearch(
	    ["elastic:CNEoDDaCEUiFoIfaZHO1xOaf@22db972c583d420b9ac09b474056156c.us-east-1.aws.found.io:9243/"],
	    scheme="https",
	    request_timeout=30
	)

def lambda_handler(event, context):
    https = urllib3.PoolManager()

    jsonEvent = json.dumps(event)
    jsonHeaders = json.loads(jsonEvent)["headers"]
    jsonData = (jsonHeaders)["data"]
    jsonData = json.loads(jsonData)
    regTime = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S%z")
    jsonData["@timestamp"] = regTime
    if(int(jsonData["measurements"]["temperature"]["ground"]) > 35):
        r = https.request('PUT', 'https://999nmxzw1g.execute-api.us-east-1.amazonaws.com/API/EmailSend')
        print("email response",r)
    print(jsonData)
    resp = es.index(index='rovyle',  body=jsonData)
    print (resp)
    return {
        'statusCode': 200,
        'body': json.dumps("indexed")
    }
