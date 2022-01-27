



import urllib.request,urllib.error,json
READ_API_KEY='Z46UJ6Q654AXJCSR'
CHANNEL_ID=1632638
def main():
    conn = urllib.request.urlopen("http://api.thingspeak.com/channels/%s/feeds/last.json?api_key=%s" \
                           % (CHANNEL_ID,READ_API_KEY))

    response = conn.read()
    print("http status code=%s" % (conn.getcode()))
    data=json.loads(response)
    print(data['field1'],data['created_at'])
    conn.close()

if __name__ == '__main__':
    main()