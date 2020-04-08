import sys
import glob
import serial
import serial
import time
import struct # Used for bin to float

# Check current connected serial ports ******************************************************************
def serial_ports():
    # Lists serial port names
    #   :raises EnvironmentError:
    #        On unsupported or unknown platforms
    #    :returns:
    #        A list of the serial ports available on the system
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


#if __name__ == '__main__':
#    print(serial_ports())

# *******************************************************************************************************
BAUDRATE = 9600
LoraSerial = serial.Serial('COM4', baudrate = BAUDRATE)
LoraSerial.timeout = 1  # set read timeout

def bin_to_float(bin_32): # 32 Bit IEEE 754 Converter
    SHIFT_SIGN = 31
    SHIFT_EXPONENT = 23
     
    MASK_SIGN     = 0b1
    MASK_EXPONENT = 0b11111111
    MASK_MANTISSA = 0b11111111111111111111111
    
    # Sign of number
    sign = (bin_32 >> SHIFT_SIGN) & MASK_SIGN 
    if(sign == 0):
       sign = 1
    else:
       sign = -1

    # Exponential of floating number
    exp = (bin_32 >> SHIFT_EXPONENT) & MASK_EXPONENT 
    exp = exp - 127

    # Mantissa
    mantissaBin = bin_32 & MASK_MANTISSA
    mantissa = 0
    m_size = 23
    for i in range(m_size):
        i = i + 1
        tmpBit = (mantissaBin >> (23 - i)) & 0b1
        mantissa = mantissa + (tmpBit / (2**i))

    mantissa = mantissa + 1

    # Float number
    result = sign * (2**exp) * mantissa
    #print("Sign: ",sign, "Exp: ", exp, "Mantissa: ", mantissa)
    #print("Result: ", result)

    return result

if LoraSerial.is_open:
    while True:
        size = LoraSerial.inWaiting()
        if size:
            data = LoraSerial.read(size)

            # Check Length
            if(len(data) >= 16):
                # Check Byte
                if(data[0] == 36) : 
                    # Process Data from here
                    hour = data[1] 
                    minute = data[2]
                    second = data[3]
                    b_lat = (data[7] << 24) ^ (data[6] << 16) ^ (data[5] << 8) ^ (data[4])
                    b_long = (data[11] << 24) ^ (data[10] << 16) ^ (data[9] << 8) ^ (data[8])
                    b_alt = (data[15] << 24) ^ (data[14] << 16) ^ (data[13] << 8) ^ (data[12])
                
                    print("Hour: {}, Minute: {}, Second: {}".format(hour, minute, second))
                    print("Float Latitude: ", bin_to_float(b_lat))
                    print("Float Longtitude: ", bin_to_float(b_long))
                    print("Float Altitude: ", bin_to_float(b_alt))

            print(" Over ")
        else:
            print('no data')
        time.sleep(1)
else:
    print('Serial not open')
LoraSerial.close()  # close z1serial if z1serial is open.
