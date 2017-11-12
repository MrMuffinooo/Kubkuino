import sys
import serial
import serial.tools.list_ports
from PyQt4 import QtGui
import winreg as reg
from itertools import count
from bluetooth import *


class Example(QtGui.QMainWindow):
    
    def __init__(self):
        super(Example, self).__init__()
        
        self.initUI()
        
        
    def initUI(self):               
        
        self.statusBar().showMessage('Ready')
        
        self.setGeometry(960, 540, 400, 300)
        self.setWindowTitle('Statusbar')    
        self.show()


def main():
    port = list(serial.tools.list_ports.comports())
    for a in port:
        print(a[2])

    key = reg.OpenKey(reg.HKEY_LOCAL_MACHINE, 'HARDWARE\\DEVICEMAP\\SERIALCOMM')
    try:
        for i in count():
            device, port = reg.EnumValue(key, i)[:2]
            print(reg.EnumValue(key,i))
              
    except WindowsError:
        pass
        
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
