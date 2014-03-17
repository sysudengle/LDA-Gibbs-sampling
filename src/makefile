CC=g++

all: lda

FileUtil.o: FileUtil.cc FileUtil.h
	$(CC) -c FileUtil.cc -o FileUtil.o

WordTrimer.o: WordTrimer.cc WordTrimer.h
	$(CC) -c WordTrimer.cc -o WordTrimer.o

WordFilter.o: WordFilter.cc WordFilter.h
	$(CC) -c WordFilter.cc -o WordFilter.o

Document.o: Document.cc Document.h WordFilter.o WordTrimer.o
	$(CC) -c Document.cc -o Document.o

Documents.o: Documents.cc Documents.h Document.o WordFilter.o WordTrimer.o FileUtil.o
	$(CC) -c Documents.cc -o Documents.o

LdaGibbsSampling.o: LdaGibbsSampling.cc LdaGibbsSampling.h Documents.o FileUtil.o
	$(CC) -c LdaGibbsSampling.cc -o LdaGibbsSampling.o

lda: runLdaModel.cc LdaGibbsSampling.o Documents.o
	$(CC) -I/usr/local/boost_1_55_0/stage/lib runLdaModel.cc LdaGibbsSampling.o Documents.o Document.o WordFilter.o WordTrimer.o FileUtil.o -o lda -lboost_regex 
