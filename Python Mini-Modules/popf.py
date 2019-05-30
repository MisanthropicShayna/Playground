import poplib, bytef

class email:
    subject = None
    message = None
    sender = None
    recipient = None
    date = None
    mailHash = None
    raw = None
    encoding = None

    def __init__(self, rawmail):
        maildata = [line.decode() for line in rawmail[1]]
        try:
            self.raw = maildata

            for line in maildata:
                if line.lower().startswith("subject:"):
                    self.subject = line[line.index(":")+1:]
                if line.lower().startswith("to:"):
                    self.recipient = line[line.index(":")+1:]
                if line.lower().startswith("date:"):
                    self.date = line[line.index(":")+1:]
                if line.lower().startswith("from:"):
                    self.sender = line[line.index(":")+1:]
                if line.lower().startswith("Content-Transfer-Encoding:"):
                    self.encoding = line[line.index(":")+1:]

            message = ""
            for i in range(len(maildata)):
                line = maildata[i]
                if line == "":
                    for line2 in maildata[i+1:]:
                        if line2 == "": message += "\n"
                        else: message += line2
                    break

            self.message = message
        except:
            return

        dmal_builder = []
        for raw_line in rawmail[1]:
            for dm in bytef.bytesToDmal(raw_line):
                dmal_builder.append(dm)

        dmal_bytes = bytes(dmal_builder)
        self.mailHash = bytef.SHA256(dmal_bytes)

class pop3:
    initialized = False
    popConnector = None

    def connect(self, address, emailUsrname, password):
        try: self.popConnector.quit()
        except: pass

        self.popConnector = poplib.POP3(address)
        self.popConnector.user(emailUsrname)
        self.popConnector.pass_(password)
        self.initialized = True

    def messageCount(self):
        if not self.initialized:
            raise Exception("Uninitialized.")

        return int(self.popConnector.list()[0].decode().split(" ")[1])
    
    def getMessage(self, messageNumber):
        if not self.initialized:
            raise Exception("Uninitialized.")

        return email(self.popConnector.retr(messageNumber))

    def terminate(self):
        if not self.initialized:
            raise Exception("Uninitialized.")

        self.popConnector.quit()


    def subjectList(self):
        if not self.initialized:
            raise Exception("Uninitialized.")
        
        subjects = []
        for i in range(self.messageCount()):
            i_mail = self.getMessage(i+1)
            subjects.append((str(i+1), i_mail.subject))

        return subjects
