import re

fileName = "/home/dengle/lda/LDAGibbsSampling-master/src/liuyang/nlp/lda/com/Stopwords.java"
writeFileName = "/home/dengle/lda/DL_LDA/data/filterWords/stopWords"

lines = [line.rstrip() for line in open(fileName)]
#pattern = re.compile(r'add[a-zA-Z]+')
pattern = re.compile(r'^(?!.*//).*add\(".*"\);')
writeFile = open(writeFileName, "w")

for line in lines:
	match = pattern.search(line)
	if match:
		str = match.string
		str = str.split("\"")
		print str[1]
		writeFile.write(str[1] + '\n')
	
writeFile.close()
