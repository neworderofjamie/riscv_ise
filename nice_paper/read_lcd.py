import csv
import cv2
import time
from subprocess import call, check_output, CalledProcessError
from tempfile import NamedTemporaryFile
cap = cv2.VideoCapture(1)

THRESHOLD = 26

start_time = time.time()
with open("power.csv", 'w') as csv_file, NamedTemporaryFile(suffix=".png", delete=False) as tmp_file:
    print(tmp_file.name)
    linux_name = tmp_file.name.replace("\\", "/")
    linux_name = linux_name.replace("C:", "/mnt/c")
    
    csv_writer = csv.writer(csv_file, delimiter=',')
    csv_writer.writerow(["Time [s]", "Power [W]"])
    while True:
        frame = cap.read()
        if frame[0]:
            cv2.imshow("frame", frame[1])
            
            
            digit = frame[1][250:400,300:450:]
            cv2.imwrite(tmp_file.name, digit)
            cv2.imshow("digit", digit)
            cv2.waitKey(1)

            try:
                output = check_output(["wsl", "ssocr", "--charset=decimal", "--number-digits=-1", f"--threshold={THRESHOLD}", linux_name]).decode("utf-8")
                print(output)
                if output.count(".") == 1 and output.count("-") == 0:
                    power = float(output)
                    print("%fW" % power)
                    csv_writer.writerow([time.time() - start_time, power])
                else:
                    print("Parser output invalid: '%s'" % output)
            except CalledProcessError as err:
                if err.returncode == 2:
                    print("Unable to recognise digit '%s'" % err.output)
                    pass
                else:
                    print(err.returncode)
                    raise
