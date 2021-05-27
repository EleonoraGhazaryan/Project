all: crawler.cpp
		g++ crawler.cpp -o crawler -L /usr/local/lib -leasyhttp -lPocoFoundation -lPocoNet -lPocoCrypto -lssl -lPocoData -lPocoNetSSL -lPocoUtil -lPocoJSON -lPocoDataSQLite 