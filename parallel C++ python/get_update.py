import threading
import time
from pyrebase import pyrebase
from collections.abc import Mapping

config = {  # define a dictionary named config with several key-value pairs that configure the connection to the database.
    "apiKey": "AIzaSyAhabvmLuMQH8Hw7TKKpBhS6kiWhdps1q0",
    "authDomain": "pixled-17de5.firebaseapp.com",
    "databaseURL": "https://pixled-17de5-default-rtdb.europe-west1.firebasedatabase.app/",
    "storageBucket": "pixled-17de5.appspot.com"
}
firebase = pyrebase.initialize_app(config)
db = firebase.database()
first_event = True

def handle_change(event):
    pixel_data = event["data"]
    pixel_path = (event["path"])
    event_information = [pixel_path[-1], pixel_data]
    print("hello")
    if (pixel_path == '/'):
        print("matrix")
        print('\0')
    else:
        print("info"+'\0')
    my_stream = db.child("ledMatrix").stream(handle_change)

def print_message():
    while True:
        print("NN"+'\0')
        time.sleep(3Ability to change leds on the physical led matrix.
Web application to control pixLED.
Use PixLED simultaneously with other people.
Use PixLED without having to log in.
See history of changes
Admin mode

        
               



my_stream = db.child("ledMatrix").stream(handle_change)
print("Started")
print('\0')
#thread = threading.Thread(target=print_message)
#thread.start()