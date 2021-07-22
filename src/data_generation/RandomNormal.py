from numpy.random import normal as RandomData

INPUT_FILE_PATH, INPUT_FILE = "../src/data_generation/RandomNormal_InputData.txt", "RandomNormal_InputData.txt"
OUTPUT_FILE_PATH, OUTPUT_FILE = "../src/data_generation/RandomNormal_GaussianData.txt", "RandomNormal_GaussianData.txt"

try: File = open(INPUT_FILE_PATH, "r")
except FileNotFoundError: File = open(INPUT_FILE, "r")

FileContent1, FileContent2, FileContent3 = File.readlines()
Mu = float(FileContent1.replace('\n', ''))
Sigma = float(FileContent2.replace('\n', ''))
Size = int(FileContent3.replace("\n", ""))

def GenerateRandomGaussianData(Mu, Sigma, Size): return RandomData(Mu, Sigma, Size)
X = GenerateRandomGaussianData(Mu, Sigma, Size)

STR_X = ""
for x in X: STR_X += str(x) + '\n'

try: File = open(OUTPUT_FILE_PATH, "w")
except FileNotFoundError: File = open(OUTPUT_FILE, "w")

File.write(STR_X)
File.close()
