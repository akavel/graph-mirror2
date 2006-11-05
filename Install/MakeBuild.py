from ftplib import FTP
from getpass import getpass
from VersionInfo import CalcVersionInfo
import os

print "Updating Graph.iss ..."
File = open("Graph.iss")
Lines = File.readlines()
Count = 0
for Line in Lines:
	if Line[:18] == "VersionInfoVersion":
		Lines[Count] = "VersionInfoVersion=" + CalcVersionInfo("Graph.exe") + "\n"
	Count += 1

File = open("Graph.iss", "w")
File.writelines(Lines)
File.close()

# Compile SetupGraphBeta.exe
print "Compiling SetupGraphBeta.exe ..."
os.system('"c:\progra~1\\Inno Setup 5\\iscc.exe" /Q Graph.iss')

# Upload SetupGraphBeta.exe to the server
ftp = FTP('ftp.padowan.dk')   # connect to host, default port
ftp.login('padowan.dk', getpass())
print "Uploading SetupGraphBeta.exe ..."
ftp.cwd('graph')
File=open('SetupGraphBeta.exe', 'rb')
ftp.storbinary('STOR SetupGraphBeta.exe', File)
ftp.quit()
print "Upload complete!"