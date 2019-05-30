from datetime import datetime
import re

MonthTable:dict = {
    "Jan":1, "Feb":2, "Mar":3, "Apr":4, "May":5, "June":6,
    "July":7,"Aug":8,"Sept":9, "Oct":10,"Nov":11,"Dec":12
}

class WarningReport:
    def __init__(self):
        self.issue_date_string:str = None
        self.issue_date:datetime = None

    def parse_report(self, report:str):
        # Go through every line in the report.
        for line in report.split("\n"):
            # Regex that matches any report issue dates on the current line.
            # Example match: 755 AM CDT Wed Apr 17 2019
            issue_date_matches = re.search(r"\d{3,4} (AM|PM) CDT .+? .+? \d{1,2} \d{4}", line)
            
            # Regex that matches report expiration dates on the current line.
            # Example match: * Until 845 AM CDT.
            expiration_date_matches = re.search(r"*Until \d{3,4} (AM|PM) CDT", line)

            # If a match for an issue date was found on the current line, process it.
            if issue_date_matches != None:
                self.issue_date_string = issue_date_matches.group(0)

                issue_date_hour, issue_date_minutes, issue_date_12notation, \
                issue_date_day, issue_date_month, issue_date_year = \
                (
                    self.issue_date_string.split(" ")[0].rjust(4, "0")[:2],
                    self.issue_date_string.split(" ")[0][:-2]
                    "",
                    "",
                    "",
                    "",
                    ""
                )
                
            # If a match for an expiration date was found on the current line, process it.
            if expiration_date_matches != None:
                pass



with open("warning.txt") as io:
    data = io.read()
    io.close()

    report = WarningReport()
    report.parse_report(data)