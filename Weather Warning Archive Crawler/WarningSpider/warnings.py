import urllib.request, threading, datetime, json, time, os, re

def generate_warning_pattern(warning_categories:tuple):
    current_date = datetime.datetime.today()

    regular_expression_pattern:str = "{0}{1:{c}>{n}}{2:{c}<{n}}{3}".format(
        str(current_date.year),
        str(current_date.month),
        str(current_date.day),
        r"\d{2}\.(%s)" % "|".join(warning_categories),
        c = "0", n = 2
    )

    return regular_expression_pattern

class Report:
    ReportLines:list = None
    ReportString:str = None

    TimeFromPattern:str = r"\d{3,4} (AM|PM) CDT .+? .+? \d{1,2} \d{4}"

    TimeFrom  = None
    TimeTo = None

    MonthTable = {
        "Jan":1, "Feb":2, "Mar":3, "Apr":4, "May":5, "June":6,
        "July":7,"Aug":8,"Sept":9, "Oct":10,"Nov":11,"Dec":12
    }

    def ParseReport(self, report:str):
        self.ReportString = report
        self.ReportLines = report.split("\n")
        
        for line in self.ReportLines:
            timeFromMatch = re.search(self.TimeFromPattern, line)
            if timeFromMatch != None:
                lineSplit = line.split(" ")
                
                self.TimeFrom = datetime.datetime(
                    int(lineSplit[6]),
                    self.MonthTable[lineSplit[4]],
                    int(lineSplit[5]),
                    int(lineSplit[0].rjust(4, "0")[:2]),
                    int(lineSplit[0].rjust(4, "0")[2:4]),
                )

class ReportIndex:
    WarningTypes = ("TOR", "SVR",  "FFS", "SVS")
    ReportIndexUrl:str = ""
    ReportIndex:dict = {}

    def GenerateReportNamePattern(self, warning_categories:tuple):
        current_date = datetime.datetime.today()

        regular_expression_pattern:str = "{0}{1:{c}>{n}}{2:{c}<{n}}{3}".format(
            str(current_date.year),
            str(current_date.month),
            str(current_date.day),
            r"\d{2}\.(%s)" % "|".join(warning_categories),
            c = "0", n = 2
        )

        return regular_expression_pattern

    def GetAllReportNames(self):
        report_names:list = []

        try:
            http_source:str = urllib.request.urlopen(self.ReportIndexUrl, timeout = 3).decode()
        except Exception as exception:
            print("GetAllReportNames !>", str(exception))
        else:
            for line in http_source.split("\n"):
                report_name:str = re.search(self.GenerateReportNamePattern(self.WarningTypes), line)

                if report_name != None:
                    report_names.append(report_name.group(0))



    def __init__(self, report_index_url:str):
        self.ReportIndexUrl = report_index_url
        self.DataBase = {}
        

def start_fetching(url:str = "http://warnings.allisonhouse.com/", warning_catrgories=("TOR", "SVR")):
    reports:dict = {}

    scheduled_check:int = int(time.time())

    while True:
        if int(time.time()) > scheduled_check:
            try:
                site_http_source = urllib.request.urlopen(url, timeout = 3).read().decode()
            except Exception as exception:
                print("Open Report Listing !>", str(exception))
            else:
                report_name_pattern = generate_warning_pattern(warning_catrgories)

                for line in site_http_source.split("\n"):
                    match = re.search(report_name_pattern, line)

                    if match != None:
                        report_name = match.group(0)
                        
                        if report_name not in reports:
                            report_content:str = None

                            for i in range(3):
                                try:
                                    report_content:str = urllib.request.urlopen(url+report_name, timeout = 3).read().decode()
                                except Exception as exception:
                                    print("Open Report !>", str(exception))
                                else:
                                    reports[report_name] = report_content
                                    break
                
                current_date = datetime.datetime.today()

                current_time_string:str = "{0}{1}{2}".format(
                    str(current_date.year),
                    str(current_date.month).zfill(2),
                    str(current_date.day).zfill(2)
                )

                reports = {k:v for (k,v) in reports.items() if k.startswith(current_time_string)}
                scheduled_check = int(time.time()) + 60
        
        os.system("cls")
        print("-- ACTIVE (%s) WARNINGS -- " % "|".join(warning_catrgories))
        for report in reports:
            category = {
                "TOR":"TORNADO", 
                "SVR":"SEVERE WEATHER",
            }[report.split(".")[-1]]

            locations = []

            if "ohio" in reports[report].lower():
                locations.append("Ohio")

            if "kentucky" in reports[report].lower():
                locations.append("Kentucky")

            if "indiana" in reports[report].lower():
                locations.append("Indiana")

            if "oklahoma" in reports[report].lower():
                locations.append("Oklahoma")

            if "virginia" in reports[report].lower():
                locations.append("Virginia")

            if "illinois" in reports[report].lower():
                locations.append("Illinois")
            
            if "tennessee" in reports[report].lower():
                locations.append("Tennessee")

            if "texas" in reports[report].lower():
                locations.append("Texas")

            if "kansas" in reports[report].lower():
                locations.append("Kansas")

            if "iowa" in reports[report].lower():
                locations.append("Iowa")

            if not len(locations):
                locations.append("OUT OF TORNADO ALLEY")
            
            year, month, day, ident = report[:4], report[4:6], report[6:8], report[8:10]
            print("-{6}- {0} @ {1}/{2}/{3} [{4}] // {5} --".format(
                category, 
                year, 
                month, 
                day, 
                ident, 
                " | ".join(locations), 
                "ONGOING" if "cancelled" not in reports[report].lower() else "CANCELLED"
                )
            )
        
        time.sleep(3)


with open("warning.txt", "r") as io:
    report_content = io.read()
    io.close()

    report = Report()
    report.ParseReport(report_content)

    print(report.TimeFrom)
    print(report.TimeTo)