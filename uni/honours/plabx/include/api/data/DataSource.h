#ifndef DATASOURCE_H_
#define DATASOURCE_H_

#include <string>

class DataSource
{

protected:
	DataSource( int sourceType ) : sourceType( sourceType ) { };
public:
	virtual ~DataSource() { };

public:
	static DataSource createLocalFileSource( std::string path );
	static DataSource createOnlineSource( std::string url );
	static DataSource createStdinSource( );
	// static DataSource createDatabaseSource( std::string dsn, std::string query );

public:
	enum
	{
		LOCAL_FILE,
		ONLINE,
		STDIN
		// DATABASE
	};

public:
	inline int getSourceType() { return this->sourceType; };

	inline std::string getLocalFilePath() { return this->localFile_path; };
	inline std::string getOnlineUrl() { return this->online_url; };
	// std::string getDatabaseDsn();
	// std::string getDatabaseQuery();

private:
	int sourceType;

	// Each prefixed with the source type they belong to...
	std::string localFile_path;
	std::string online_url;
	// std::string database_dsn;
	// std::string database_query;

};

#endif /* DATASOURCE_H_ */
