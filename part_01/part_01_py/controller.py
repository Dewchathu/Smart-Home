import urllib.request
import urllib.error


main_url = "http://192.168.8.124"  # Update this with the IP of your ESP8266 server


def sendRequest(url):
    try:
        urllib.request.urlopen(url)
    except urllib.error.URLError as e:
        print(f"Failed to open URL: {url}")
        print(f"Error: {e}")


def ledon():
    sendRequest(main_url + "/ledon")


def ledoff():
    sendRequest(main_url + "/ledoff")


def ledCon(total):
    try:
        if total == 5:
            ledon()
        elif total == 0:
            ledoff()
    except Exception as e:
        print(f"An error occurred: {e}")

# Add any other functions or logic as needed
