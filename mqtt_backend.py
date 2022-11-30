# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer

hostName = "localhost"
serverPort = 8081

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/plain")
        f = open("www/temp.txt", "r")
        lines = f.readlines()
        self.send_header("Content-type", "text/plain")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        if lines:
            self.wfile.write(bytes('{}'.format(lines[-1]), "utf-8"))
        else:
            self.wfile.write(bytes('0', "utf-8"))
        

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")