/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

/*
 *   Include directly the different
 *     headers from cppconn/ and mysql_driver.h + mysql_util.h
 *       (and mysql_connection.h). This will reduce your build time!
 *       */
//#include "mysql_connection.h"

#include <Freeze/Freeze.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "MapAlerts.h"

using namespace std;

int main(int argc, char* argv[]){
	// set properties for db ice
	Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
	initData.properties->load("config");

	// initialize che Ice communicator
	Ice::CommunicatorPtr communicator = Ice::initialize(argc,argv,initData);

	// create the Freeze's db with the BDB in disk
	Freeze::ConnectionPtr connection = Freeze::createConnection(communicator, "db");

	// inizialize the map for freeze
	MapAlerts mapAl(connection, "MapAlerts");

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		Struttura::alerts tmp;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("localhost", "student1", "AGILE2013science");
		/* Connect to the MySQL test database */
		con->setSchema("agilesas");

		stmt = con->createStatement();
		string query = "SELECT alerts . * , skyregions.name AS skyregionname, sciencealertsystems.name AS sciencealertname ";
			query += "FROM alerts, skyregions, sciencealertsystems ";
			query += "WHERE alerts.skyregion_id = skyregions.id ";
			query += "AND alerts.sciencealert_id = sciencealertsystems.id";
		res = stmt->executeQuery(query);
		std::cout << "l_peack\tb_peak\tt_start_mjd\tt_stop_mjd\tskyregion_id\tsciencealert_id\tsi_index\tfix_flag\tsrcloc_conf_level\tul_conf_level\tgal";
		std::cout << "gale_err\tiso\tiso_err\tlabel\tsqrt_ts\tcounts\tcounts_err\tcounts_ul\tflux\tflux_err\tflux_UL\tspectral_index\tspectral_index_error\t";
		std::cout << "l\tb\tr\tell_a\tell_b\tell_phi\tpath\tt_start_tt\tt_stop_tt\tt_start_o\tt_stop_o\tt_Start_utc\tt_stop_utc\texposoure\tid" << std::endl;
		while (res->next()) {
			tmp.lpeak = res->getDouble("l_peak");
			tmp.bpeak = res->getDouble("b_peak");
			tmp.tstartmjd = res->getDouble("t_start_mjd");
			tmp.tstopmjd = res->getDouble("t_stop_mjd");
			tmp.skyregionid = res->getInt("skyregion_id");
			//tmp.skyregionname = res->getString("l_peak");
			tmp.sciencealertid = res->getInt("sciencealert_id");
			//tmp.sciencealertname = res->getString("l_peak");
			tmp.siindex = res->getDouble("si_index");
			tmp.fixflag = res->getInt("fix_flag");
			tmp.srclocconflevel = res->getDouble("srcloc_conf_level");
			tmp.ulconflevel = res->getDouble("ul_conf_level");
			tmp.gal = res->getDouble("gal");
			tmp.galerror = res->getDouble("gal_err");
			tmp.iso = res->getDouble("iso");
			tmp.isoerr = res->getDouble("iso_err");
			tmp.label = res->getString("label");
			tmp.sqrtts = res->getDouble("sqrt_ts");
			tmp.counts = res->getDouble("counts");
			tmp.countserr = res->getDouble("counts_err");
			tmp.countsul = res->getDouble("counts_ul");
			tmp.flux = res->getDouble("flux");
			tmp.fluxerr = res->getDouble("flux_err");
			tmp.fluxUL = res->getDouble("flux_UL");
			tmp.spectralindex = res->getDouble("spectral_index");
			tmp.spectralindexerror = res->getDouble("spectral_index_error");
			tmp.l = res->getDouble("l");
			tmp.b = res->getDouble("b");
			tmp.r = res->getDouble("r");
			tmp.ella = res->getDouble("ell_a");
			tmp.ellb = res->getDouble("ell_b");
			tmp.ellphi = res->getDouble("ell_phi");
			tmp.path = res->getString("path");
			tmp.tstarttt = res->getDouble("t_start_tt");
			tmp.tstoptt = res->getDouble("t_stop_tt");
			tmp.tstarto = res->getInt("t_start_o");
			tmp.tstopo = res->getInt("t_stop_o");
			tmp.tstartutc = res->getString("t_start_utc");
			tmp.tstoputc = res->getString("t_stop_utc");
			tmp.exposoure = res->getDouble("exposoure");
			tmp.id = res->getInt("id");
			// insert into the map
			mapAl.insert(make_pair(tmp.id,tmp));
			// show data insert in db
			std::cout << tmp.lpeak << "\t" << tmp.bpeak << "\t" << tmp.tstartmjd << "\t" << tmp.tstopmjd << "\t" << tmp.skyregionid << "\t";
			std::cout << tmp.sciencealertid << "\t" << tmp.siindex << "\t" << tmp.fixflag << "\t" << tmp.srclocconflevel << "\t";
			std::cout << tmp.ulconflevel << "\t" << tmp.gal << "\t" << tmp.galerror << "\t" << tmp.iso << "\t" << tmp.isoerr << "\t";
			std::cout << tmp.label << "\t" << tmp.sqrtts << "\t" << tmp.counts << "\t" << tmp.countserr << "\t" << tmp.countsul << "\t";
			std::cout << tmp.flux << "\t" << tmp.fluxerr << "\t" << tmp.fluxUL << "\t" << tmp.spectralindex << "\t" << tmp.spectralindexerror;
			std::cout << "\t" << tmp.l << "\t" << tmp.b << "\t" << tmp.r << "\t" << tmp.ella << "\t" << tmp.ellb << "\t" << tmp.ellphi << "\t";
			std::cout << tmp.path << "\t" << tmp.tstarttt << "\t" << tmp.tstoptt << "\t" << tmp.tstarto << "\t" << tmp.tstopo << "\t";
			std::cout << tmp.tstartutc << "\t" << tmp.tstoputc << "\t" << tmp.exposoure << "\t" << tmp.id << std::endl;
		}
		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	cout << endl;

	return EXIT_SUCCESS;
}
