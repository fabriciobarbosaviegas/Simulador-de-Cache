#Corrige os casos de teste do documento

import subprocess

subprocess.run('make', shell=True)

inputs = {
            "LRU": ['./cache_simulator 1 4 32 L 1 tests/vortex.in.sem.persons.bin', './cache_simulator 2 1 8 L 1 tests/bin_100.bin'],
            "FIFO": ['./cache_simulator 2 1 8 F 1 tests/bin_100.bin', './cache_simulator 1 4 32 F 1 tests/vortex.in.sem.persons.bin'],
            "RANDOM": ['./cache_simulator 256 4 1 R 1 tests/bin_100.bin', './cache_simulator 128 2 4 R 1 tests/bin_1000.bin', './cache_simulator 16 2 8 R 1 tests/bin_10000.bin', './cache_simulator 512 8 2 R 1 tests/vortex.in.sem.persons.bin', './cache_simulator 1 4 32 R 1 tests/vortex.in.sem.persons.bin', './cache_simulator 2 1 8 R 1 tests/bin_100.bin', './cache_simulator 1 4 32 R 1 tests/vortex.in.sem.persons.bin']
        }

outputs = {
            "LRU": ['186676 0.5756 0.4244 0.00 1.00 0.00', '100 0.46 0.54 0.30 0.67 0.04'],
            "FIFO": ['100 0.43 0.57 0.28 0.68 0.04', '186676 0.5530 0.4470 0.00 1.00 0.00'],
            "RANDOM": ['100 0.9200 0.0800 1.00 0.00 0.00', '1000 0.8640 0.1360 1.00 0.00 0.00', '10000 0.9298 0.0702 0.18 0.79 0.03', '186676 0.8782 0.1218 0.05 0.93 0.02', '186676 0.5440 0.4560 0.00 1.00 0.00', '100 0.43 0.57 0.28 0.68 0.04', '186676 0.5440 0.4560 0.00 1.00 0.00']
        }


politica = int(input("Digite a política de substituição:\n1 - LRU\n2 - FIFO\n3 - RANDOM\n4 - Todas: "))

if(politica == 1):
    for index, j in enumerate(inputs['LRU']):
        response = subprocess.run(j, shell=True, capture_output = True, text = True) 

        print("Teste: ", j)
        print("Output esperado: ", outputs['LRU'][index])
        print("Output obtido: ", response.stdout)

elif(politica == 2):
    for index, j in enumerate(inputs['FIFO']):
        response = subprocess.run(j, shell=True, capture_output = True, text = True) 

        print("Teste: ", j)
        print("Output esperado: ", outputs['FIFO'][index])
        print("Output obtido: ", response.stdout)

elif(politica == 3):
    for index, j in enumerate(inputs['RANDOM']):
        response = subprocess.run(j, shell=True, capture_output = True, text = True) 

        print("Teste: ", j)
        print("Output esperado: ", outputs['RANDOM'][index])
        print("Output obtido: ", response.stdout)

else:
    for i in inputs:
        for index, j in enumerate(inputs[i]):
            response = subprocess.run(j, shell=True, capture_output = True, text = True) 

            print("Teste: ", j)
            print("Output esperado: ", outputs[i][index])
            print("Output obtido: ", response.stdout)

