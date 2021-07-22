from numpy.random import multivariate_normal as RandomData

INPUT_FILE_PATH, INPUT_FILE = "../src/data_generation/RandomMultivariate_InputData.txt", "RandomMultivariate_InputData.txt"
OUTPUT_FILE_PATH, OUTPUT_FILE = "../src/data_generation/RandomMultivariate_GaussianData.txt", "RandomMultivariate_GaussianData.txt"

try: File = open(INPUT_FILE_PATH, "r")
except FileNotFoundError: File = open(INPUT_FILE, "r")

FileContent1, FileContent2, FileContent3 = File.readlines()
Mu = list(map(lambda x: int(x), FileContent1.replace("\n", "").split(",")))
Sigma = list(map(lambda x: int(x), FileContent2.replace("\n", "").split(",")))
Sigma = [Sigma[0:2], Sigma[1:-1]]
Size = int(FileContent3)

def GenerateRandomGaussianData(Mu, Sigma, Size): return RandomData(Mu, Sigma, Size, check_valid='ignore')
X = GenerateRandomGaussianData(Mu, Sigma, Size)

STR_X = ""
for x in X: STR_X += str(int(x[0])) + "," + str(int(x[1])) + '\n'

try: File = open(OUTPUT_FILE_PATH, "w")
except FileNotFoundError: File = open(OUTPUT_FILE, "w")

File.write(STR_X)
File.close()
