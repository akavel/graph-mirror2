from ftplib import FTP
from getpass import getpass
from VersionInfo import CalcVersionInfo
import os
import sys
import traceback
import datetime

try:
  # Test that Graph can be started without some obscure dll files
  os.system("Graph.exe /regserver")

  print("Updating Graph.iss ...")
  File = open("Graph.iss", "rb")
  Lines = File.readlines()
  File.close()
  VersionInfo = CalcVersionInfo("Graph.exe")
  BaseFileName = "SetupGraphBeta-" + VersionInfo
  FileName = BaseFileName + ".exe"
  Count = 0
  for Line in Lines:
    if Line[:18] == b"VersionInfoVersion":
      Lines[Count] = b"VersionInfoVersion=" + VersionInfo.encode() + b"\n"
    if Line[:18] == b"OutputBaseFilename":
      Lines[Count] = b"OutputBaseFilename=" + BaseFileName.encode() + b'\n'
    Count += 1

  File = open("Graph.iss", "wb")
  File.writelines(Lines)
  File.close()

  # Compile SetupGraphBeta-4.2.0.x.exe
  print("Compiling", FileName, "...")
  os.system('"c:\progra~1\\Inno Setup 5\\iscc.exe" /Q Graph.iss')

  #Creating GraphBeta.inf
  print("Writing GraphBeta.inf ...")
  File = open("GraphBeta.inf", "w")
  File.write("[Graph]\n")
  File.write("Major = " + VersionInfo[0] + "\n")
  File.write("Minor = " + VersionInfo[2] + "\n")
  File.write("Release = " + VersionInfo[4] + "\n")
  File.write("Build = " + VersionInfo[6:] + "\n")
  File.write("Date = " + datetime.date.today().strftime("%d-%m-%Y\n"))
  File.write("DownloadFile = http://www.padowan.dk/bin/" + FileName + '\n')
  File.write("DownloadPage = http://www.padowan.dk/graph/Beta.php\n")

  # Upload SetupGraphBeta.exe to the server
  ftp = FTP('ftp.padowan.dk')   # connect to host, default port
  ftp.login('padowan.dk', getpass())
  ftp.cwd('bin')
  print("Uploading", FileName, "...")
  File = open(FileName, 'rb')
  ftp.storbinary('STOR ' + FileName, File)
  if ftp.size(FileName) != os.stat(FileName).st_size:
    raise Exception("Wrong file size on server")

  print("Uploading GraphBeta.inf ...")
  ftp.cwd('../graph')
  File = open("GraphBeta.inf", 'rb')
  ftp.storbinary('STOR GraphBeta.inf', File)

  ftp.quit()
  print("Upload complete!")
except:
  traceback.print_exception(sys.exc_info()[0], sys.exc_info()[1], sys.exc_info()[2])
  sys.stdin.readline()
