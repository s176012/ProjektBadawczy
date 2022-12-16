# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
# hostName = "172.20.98.222"
hostName = "localhost"
serverPort = 8081

class MyServer(BaseHTTPRequestHandler):
    def read_and_send_sensor_data(self):
        db = open("www/db.txt", "r")
        lines = db.readlines()
        if lines:
            last_line = lines[-1]
        if "iot/temperature" in last_line:
            last_line = last_line.split(" ")[0]+" "+str(float(last_line.split(" ")[1])/1000)
            self.send_sensor_data("temperature", last_line)
        if "iot/color" in last_line:
            self.send_sensor_data("color", last_line)
        if "iot/acc" in last_line:
            self.send_sensor_data("acc", last_line)   

    def send_sensor_data(self, sensor_type, data):
        data = data.replace("iot/{} ".format(sensor_type), "")
        self.send_header("Content-type", "text/plain")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        self.wfile.write(bytes('{}_{}'.format(sensor_type, data), "utf-8"))
    
    def do_GET(self):
        self.send_response(200)
        self.read_and_send_sensor_data()
        

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
