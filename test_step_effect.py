import subprocess
import sys

print('Usage: python app.py [cachegrind|timing]')
cmd = sys.argv[1] if len(sys.argv) > 1 else 'cachegrind' 

f = open('test_out.txt', 'w')

step = 1
while step < 200:
   test_out = subprocess.check_output(['./' + cmd  + '_test.sh '+ str(step) + ' 1000000000'], shell=True)
   step = step + 1
   f.write('\n' + test_out)

f.close()
