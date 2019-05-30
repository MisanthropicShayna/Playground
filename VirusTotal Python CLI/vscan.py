import hashlib
import urllib.request
import requests
import json
import sys
import time
from os.path import *

key = ""


prs = sys.argv[1:]

if prs[0] == "-h":
	url = "https://www.virustotal.com/vtapi/v2/file/report?apikey={0}&resource={1}".format(key, prs[1])
	with urllib.request.urlopen(url) as request:
		res = json.loads(request.read().decode())
		if res["response_code"]:
			print("\n" + prs[1] + "\n========================================")
			try:
				print("HASH FOUND | Ratio {0}/{1}({2}) at - {3}".format(res["positives"], res["total"], str(int(int(res["positives"]) / int(res["total"]) * 100)) + "%", res["scan_date"]))
			except:
				print("File is still being analyzed.")
				raise SystemExit
			if len(prs) >= 3:
				if prs[2] == "-v":
					print("========================================")
					for scan in res["scans"]:
						result, verbose = "CLEAN",""
						if res["scans"][scan]["detected"]:
							result = "INFECTED"
							verbose = "[" + res["scans"][scan]["result"] + "]"

						print("{0} - {1} {2}".format(scan,result,verbose))
					print("========================================")

		else:
			print("The specified hash wasn't found on the database.")

if prs[0] == "-f":
	if exists(prs[1]) and isfile(prs[1]):
		with open(prs[1],"rb") as io:
			fhash = hashlib.sha256(io.read()).hexdigest()

		url = "https://www.virustotal.com/vtapi/v2/file/report?apikey={0}&resource={1}".format(key, fhash)
		with urllib.request.urlopen(url) as request:
			res = json.loads(request.read().decode())
			if res["response_code"]:
				print("\n" + prs[1] + " (" + fhash + ")\n========================================")
				try:
					print("HASH FOUND | Ratio {0}/{1}({2}) at - {3}".format(res["positives"], res["total"], str(int(int(res["positives"]) / int(res["total"]) * 100)) + "%", res["scan_date"]))
				except:
					print("File is still being analyzed.")
					raise SystemExit
				if len(prs) >= 3:
					if prs[2] == "-v":
						print("========================================")
						for scan in res["scans"]:
							result, verbose = "CLEAN",""
							if res["scans"][scan]["detected"]:
								result = "INFECTED"
								verbose = "[" + res["scans"][scan]["result"] + "]"

							print("{0} - {1} {2}".format(scan,result,verbose))
						print("========================================")
			else:
				with open(prs[1],"rb") as io:
					fhash = hashlib.sha256(io.read()).hexdigest()
					filename = prs[1].split("\\")[-1]
					url = "https://www.virustotal.com/vtapi/v2/file/scan?apikey={0}".format(key)
					print("Uploading..")
					with requests.post("https://www.virustotal.com/vtapi/v2/file/scan", files={"file":(filename, io)}, params={"apikey":key}) as request:
						res = request.json()
						if res["response_code"]:
							print("File has been uploaded. Waiting for 15 seconds until report check..")
							time.sleep(15)
						else:
							print("An error occured in uploading the file.")
							raise SystemExit

				report, counter = False, 0
				url = "https://www.virustotal.com/vtapi/v2/file/report?apikey={0}&resource={1}".format(key, fhash)
				while not report:
					with urllib.request.urlopen(url) as request:
						res = json.loads(request.read().decode())
						if res["response_code"]:
							try:
								print("\n" + prs[1] + " (" + fhash + ")\n========================================")
								print("HASH FOUND | Ratio {0}/{1}({2}) at - {3}".format(res["positives"], res["total"], str(int(int(res["positives"]) / int(res["total"]) * 100)) + "%", res["scan_date"]))
								if len(prs) >= 3:
									if prs[2] == "-v":
										print("========================================")
										for scan in res["scans"]:
											result, verbose = "CLEAN",""
											if res["scans"][scan]["detected"]:
												result = "INFECTED"
												verbose = "[" + res["scans"][scan]["result"] + "]"

											print("{0} - {1} {2}".format(scan,result,verbose))
										print("========================================")
								report = True
							except:
								print("Report still not generated.. Waiting another 15 seconds.")
								counter += 1
								time.sleep(15)
							
						else:
							print("Report still not generated.. Waiting another 15 seconds.")
							counter += 1
							time.sleep(15)
						




