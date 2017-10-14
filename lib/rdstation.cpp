// rdstation.cpp
//
// Abstract a Rivendell Workstation.
//
//   (C) Copyright 2002-2010,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <qobject.h>

#include <rdairplay_conf.h>
#include <rdconf.h>
#include <rdstation.h>
#include <rd.h>
#include <rddb.h> 
#include <rdescape_string.h>

//
// Global Classes
//
RDStation::RDStation(const QString &name,bool create)
{
  QString sql;
  time_offset_valid = false;
  station_name=name;
}


RDStation::~RDStation()
{
//  printf("Destroying RDStation\n");
}


bool RDStation::exists() const
{
  return RDDoesRowExist("STATIONS","NAME",RDEscapeString(station_name));
}


QString RDStation::name() const
{
  return station_name;
}


QString RDStation::description() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"DESCRIPTION").toString();
}


void RDStation::setDescription(QString desc) const
{
  SetRow("DESCRIPTION",desc);
}


QString RDStation::userName() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"USER_NAME").toString();
}


void RDStation::setUserName(QString name) const
{
  SetRow("USER_NAME",name);
}


QString RDStation::defaultName() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"DEFAULT_NAME").
    toString();
}


void RDStation::setDefaultName(QString name) const
{
  SetRow("DEFAULT_NAME",name);
}


QHostAddress RDStation::address() const
{
  QHostAddress addr;
  addr.setAddress(RDGetSqlValue("STATIONS","NAME",station_name,"IPV4_ADDRESS").
		  toString());
  return addr;
}


void RDStation::setAddress(QHostAddress addr) const
{
  SetRow("IPV4_ADDRESS",addr.toString());
}


QHostAddress RDStation::httpAddress(RDConfig *config) const
{
  QHostAddress addr;
  
  addr.setAddress("127.0.0.1");
  if(httpStation()!="localhost") {
    if(httpStation()==RD_RDSELECT_LABEL) {
      addr.setAddress(config->audioStoreXportHostname());
    }
    else {
      addr.setAddress(RDGetSqlValue("STATIONS","NAME",httpStation(),
				    "IPV4_ADDRESS").toString());
    }
  }

  return addr;
}


QString RDStation::httpStation() const
{
  return
    RDGetSqlValue("STATIONS","NAME",station_name,"HTTP_STATION").toString();
}


void RDStation::setHttpStation(const QString &str)
{
  SetRow("HTTP_STATION",str);
}


QString RDStation::caeStation() const
{
  return
    RDGetSqlValue("STATIONS","NAME",station_name,"CAE_STATION").toString();
}


QHostAddress RDStation::caeAddress(RDConfig *config) const
{
  QHostAddress addr;

  addr.setAddress("127.0.0.1");
  if(caeStation()!="localhost") {
    if(caeStation()==RD_RDSELECT_LABEL) {
      addr.setAddress(config->audioStoreCaeHostname());
    }
    else {
      addr.setAddress(RDGetSqlValue("STATIONS","NAME",caeStation(),
				    "IPV4_ADDRESS").toString());
    }
  }

  return addr;
}


void RDStation::setCaeStation(const QString &str)
{
  SetRow("CAE_STATION",str);
}


QString RDStation::webServiceUrl(RDConfig *config) const
{
  return QString("http://")+httpAddress(config).toString()+
    "/rd-bin/rdxport.cgi";
}


int RDStation::timeOffset()
{
  if (!time_offset_valid){
    time_offset = RDGetSqlValue("STATIONS","NAME",station_name,"TIME_OFFSET").toInt();
    time_offset_valid = true;
  }
  return time_offset;
}


void RDStation::setTimeOffset(int msecs)
{
  SetRow("TIME_OFFSET",msecs);
  time_offset = msecs;
}


QString RDStation::backupPath() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"BACKUP_DIR").toString();
}


void RDStation::setBackupPath(QString path) const
{
  SetRow("BACKUP_DIR",path);
}


int RDStation::backupLife() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"BACKUP_LIFE").toInt();
}


void RDStation::setBackupLife(int days) const
{
  SetRow("BACKUP_LIFE",days);
}


void RDStation::setBroadcastSecurity(BroadcastSecurityMode mode)
{
  SetRow("BROADCAST_SECURITY",mode);
}


RDStation::BroadcastSecurityMode RDStation::broadcastSecurity() const
{
  return (RDStation::BroadcastSecurityMode)RDGetSqlValue("STATIONS","NAME",
                                                         station_name,
                                                         "BROADCAST_SECURITY").
                                                         toUInt();
}


unsigned RDStation::heartbeatCart() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"HEARTBEAT_CART").
    toUInt();
}


void RDStation::setHeartbeatCart(unsigned cartnum) const
{
  SetRow("HEARTBEAT_CART",cartnum);
}


unsigned RDStation::heartbeatInterval() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"HEARTBEAT_INTERVAL").
    toUInt();
}


void RDStation::setHeartbeatInterval(unsigned interval) const
{
  SetRow("HEARTBEAT_INTERVAL",interval);
}


unsigned RDStation::startupCart() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"STARTUP_CART").
    toUInt();
}


void RDStation::setStartupCart(unsigned cartnum) const
{
  SetRow("STARTUP_CART",cartnum);
}


QString RDStation::editorPath() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"EDITOR_PATH").
    toString();
}


void RDStation::setEditorPath(const QString &cmd)
{
  SetRow("EDITOR_PATH",cmd);
}


RDStation::FilterMode RDStation::filterMode() const
{
  return (RDStation::FilterMode)RDGetSqlValue("STATIONS","NAME",station_name,
					      "FILTER_MODE").toInt();
}


void RDStation::setFilterMode(RDStation::FilterMode mode) const
{
  SetRow("FILTER_MODE",(int)mode);
}


bool RDStation::startJack() const
{
  return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,"START_JACK").
		toString());
}


void RDStation::setStartJack(bool state) const
{
  SetRow("START_JACK",RDYesNo(state));
}


QString RDStation::jackServerName() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"JACK_SERVER_NAME").
    toString();
}


void RDStation::setJackServerName(const QString &str) const
{
  SetRow("JACK_SERVER_NAME",str);
}


QString RDStation::jackCommandLine() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"JACK_COMMAND_LINE").
    toString();
}


void RDStation::setJackCommandLine(const QString &str) const
{
  SetRow("JACK_COMMAND_LINE",str);
}


int RDStation::cueCard() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CUE_CARD").toInt();
}


void RDStation::setCueCard(int card)
{
  SetRow("CUE_CARD",card);
}


int RDStation::cuePort() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CUE_PORT").toInt();
}


void RDStation::setCuePort(int port)
{
  SetRow("CUE_PORT",port);
}


unsigned RDStation::cueStartCart() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CUE_START_CART").
    toUInt();
}


void RDStation::setCueStartCart(unsigned cartnum) const
{
  SetRow("CUE_START_CART",cartnum);
}


unsigned RDStation::cueStopCart() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CUE_STOP_CART").toUInt();
}


void RDStation::setCueStopCart(unsigned cartnum) const
{
  SetRow("CUE_STOP_CART",cartnum);
}


int RDStation::cartSlotColumns() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CARTSLOT_COLUMNS").
    toInt();
}


void RDStation::setCartSlotColumns(int cols)
{
  SetRow("CARTSLOT_COLUMNS",cols);
}


int RDStation::cartSlotRows() const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,"CARTSLOT_ROWS").toInt();
}


void RDStation::setCartSlotRows(int rows)
{
  SetRow("CARTSLOT_ROWS",rows);
}


bool RDStation::enableDragdrop() const
{
  return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
			      "ENABLE_DRAGDROP").toString());
}


void RDStation::setEnableDragdrop(bool state)
{
  SetRow("ENABLE_DRAGDROP",state);
}


bool RDStation::enforcePanelSetup() const
{
  return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
			      "ENFORCE_PANEL_SETUP").toString());
}


void RDStation::setEnforcePanelSetup(bool state)
{
  SetRow("ENFORCE_PANEL_SETUP",state);
}


bool RDStation::systemMaint() const
{
  return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,"SYSTEM_MAINT").
	       toString());
}


void RDStation::setSystemMaint(bool state) const
{
  SetRow("SYSTEM_MAINT",state);
}


bool RDStation::scanned() const
{
  return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,"STATION_SCANNED").
	       toString());
}


void RDStation::setScanned(bool state) const
{
  SetRow("STATION_SCANNED",state);
}


bool RDStation::haveCapability(Capability cap) const
{
  switch(cap) {
      case RDStation::HaveOggenc:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_OGGENC").toString());
	break;

      case RDStation::HaveOgg123:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_OGG123").toString());
	break;

      case RDStation::HaveFlac:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_FLAC").toString());
	break;

      case RDStation::HaveLame:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_LAME").toString());
	break;

      case RDStation::HaveMp4Decode:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_MP4_DECODE").toString());

      case RDStation::HaveMpg321:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_MPG321").toString());

      case RDStation::HaveTwoLame:
	return RDBool(RDGetSqlValue("STATIONS","NAME",station_name,
				  "HAVE_TWOLAME").toString());
	break;
  }
  return false;
}


void RDStation::setHaveCapability(Capability cap,bool state) const
{
  switch(cap) {
      case RDStation::HaveOggenc:
	SetRow("HAVE_OGGENC",state);
	break;

      case RDStation::HaveOgg123:
	SetRow("HAVE_OGG123",state);
	break;

      case RDStation::HaveFlac:
	SetRow("HAVE_FLAC",state);
	break;

      case RDStation::HaveLame:
	SetRow("HAVE_LAME",state);
	break;

      case RDStation::HaveMp4Decode:
	SetRow("HAVE_MP4_DECODE",state);
	break;

      case RDStation::HaveMpg321:
	SetRow("HAVE_MPG321",state);
	break;

      case RDStation::HaveTwoLame:
	SetRow("HAVE_TWOLAME",state);
	break;
  }
}


int RDStation::cards() const
{
  int n=0;

  QString sql=QString().sprintf("select CARD0_DRIVER,CARD1_DRIVER,\
                                 CARD2_DRIVER,CARD3_DRIVER,CARD4_DRIVER,\
                                 CARD5_DRIVER,CARD6_DRIVER,CARD7_DRIVER\
                                 from STATIONS where NAME=\"%s\"",
				(const char *)station_name);
  RDSqlQuery *q=new RDSqlQuery(sql);
  if(q->first()) {
    for(int i=0;i<RD_MAX_CARDS;i++) {
      if((RDStation::AudioDriver)q->value(i).toInt()!=RDStation::None) {
	n++;
      }
      else {
	delete q;
	return n;
      }
    }
  }
  delete q;

  return n;
}


RDStation::AudioDriver RDStation::cardDriver(int cardnum) const
{
  return (RDStation::AudioDriver)
    RDGetSqlValue("STATIONS","NAME",station_name,
		 QString().sprintf("CARD%d_DRIVER",cardnum)).
    toInt();
}


void RDStation::setCardDriver(int cardnum,AudioDriver driver) const
{
  SetRow(QString().sprintf("CARD%d_DRIVER",cardnum),(int)driver);
}


QString RDStation::driverVersion(AudioDriver driver) const
{
  switch(driver) {
      case RDStation::None:
	return QString();

      case RDStation::Hpi:
	return RDGetSqlValue("STATIONS","NAME",station_name,"HPI_VERSION").
	  toString();

      case RDStation::Jack:
	return RDGetSqlValue("STATIONS","NAME",station_name,"JACK_VERSION").
	  toString();

      case RDStation::Alsa:
	return RDGetSqlValue("STATIONS","NAME",station_name,"ALSA_VERSION").
	  toString();
  }
  return QString();
}


void RDStation::setDriverVersion(AudioDriver driver,QString ver) const
{
  switch(driver) {
      case RDStation::None:
	break;

      case RDStation::Hpi:
	SetRow("HPI_VERSION",ver);
	break;

      case RDStation::Jack:
	SetRow("JACK_VERSION",ver);
	break;

      case RDStation::Alsa:
	SetRow("ALSA_VERSION",ver);
	break;
  }
}


QString RDStation::cardName(int cardnum) const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,
		      QString().sprintf("CARD%d_NAME",cardnum)).toString();
}


void RDStation::setCardName(int cardnum,QString name) const
{
  SetRow(QString().sprintf("CARD%d_NAME",cardnum),name);
}


int RDStation::cardInputs(int cardnum) const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,
		      QString().sprintf("CARD%d_INPUTS",cardnum)).toInt();
}


void RDStation::setCardInputs(int cardnum,int inputs) const
{
  SetRow(QString().sprintf("CARD%d_INPUTS",cardnum),inputs);
}


int RDStation::cardOutputs(int cardnum) const
{
  return RDGetSqlValue("STATIONS","NAME",station_name,
		      QString().sprintf("CARD%d_OUTPUTS",cardnum)).toInt();
}


void RDStation::setCardOutputs(int cardnum,int outputs) const
{
  SetRow(QString().sprintf("CARD%d_OUTPUTS",cardnum),outputs);
}


bool RDStation::create(const QString &name,QString *err_msg,
		       const QString &exemplar,const QHostAddress &hostaddr)
{
  QString sql;
  RDSqlQuery *q;
  RDSqlQuery *q1;
  QHostAddress addr=hostaddr;

  if(addr.isNull()) {
    addr.setAddress("127.0.0.1");
  }

  if(exemplar.isEmpty()) {  // Create Blank Host Config
    sql=QString("insert into STATIONS set ")+
      "NAME=\""+RDEscapeString(name)+"\","+
      "IPV4_ADDRESS=\""+RDEscapeString(addr.toString())+"\","+
      "DESCRIPTION=\"Workstation "+RDEscapeString(name)+"\","+
      "USER_NAME=\"user\","+
      "DEFAULT_NAME=\"user\"";
    q=new RDSqlQuery(sql);
    if(!q->isActive()) {
      *err_msg=QObject::tr("Host already exists!");
      delete q;
      return false;
    }
    delete q;

    //
    // Create Service Perms
    //
    sql=QString("select NAME from SERVICES");
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into SERVICE_PERMS set ")+
	"SERVICE_NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	"STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // RDAirPlay/RDPanel Channel Data
    //
    for(unsigned i=0;i<10;i++) {
      sql=QString("insert into RDAIRPLAY_CHANNELS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("INSTANCE=%u",i);
      q=new RDSqlQuery(sql);
      delete q;

      sql=QString("insert into RDPANEL_CHANNELS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("INSTANCE=%u",i);
      q=new RDSqlQuery(sql);
      delete q;
    }
    for(unsigned i=0;i<RD_CUT_EVENT_ID_QUAN;i++) {
      for(unsigned j=0;j<MAX_DECKS;j++) {
	sql=QString("insert into DECK_EVENTS set ")+
	  "STATION_NAME=\""+RDEscapeString(name)+"\","+
	  QString().sprintf("CHANNEL=%u,",j+129)+
	  QString().sprintf("NUMBER=%u",i+1);
	q=new RDSqlQuery(sql);
	delete q;
      }
    }

    //
    // RDAirPlay Log Modes
    //
    for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
      sql=QString().sprintf("insert into LOG_MODES set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("MACHINE=%d",i);
      q=new RDSqlQuery(sql);
      delete q;
    }
  }
  else {    // Use Template Host
    sql=QString("select ")+
      "DEFAULT_NAME,"+        // 00
      "STARTUP_CART,"+        // 01
      "TIME_OFFSET,"+         // 02
      "BROADCAST_SECURITY,"+  // 03
      "HEARTBEAT_CART,"+      // 04
      "HEARTBEAT_INTERVAL,"+  // 05
      "EDITOR_PATH,"+         // 06
      "FILTER_MODE,"+         // 07
      "SYSTEM_MAINT,"+        // 08
      "HTTP_STATION,"+        // 09
      "CAE_STATION,"+         // 10
      "BACKUP_DIR,"+          // 11
      "BACKUP_LIFE,"+         // 12
      "START_JACK,"+          // 13
      "JACK_SERVER_NAME,"+    // 14
      "JACK_COMMAND_LINE,"+   // 15
      "CUE_CARD,"+            // 16
      "CUE_PORT,"+            // 17
      "CUE_START_CART,"+      // 18
      "CUE_STOP_CART,"+       // 19
      "CARTSLOT_COLUMNS,"+    // 20
      "CARTSLOT_ROWS,"+       // 21
      "ENABLE_DRAGDROP,"+     // 22
      "ENFORCE_PANEL_SETUP "+ // 23
      "from STATIONS where "+
      "NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into STATIONS set ")+
	"NAME=\""+RDEscapeString(name)+"\","+
	"IPV4_ADDRESS=\""+RDEscapeString(addr.toString())+"\","+
	"DESCRIPTION=\""+RDEscapeString("Workstation "+name)+"\","+
	"USER_NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	"DEFAULT_NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	QString().sprintf("STARTUP_CART=%u,",q->value(1).toUInt())+
	QString().sprintf("TIME_OFFSET=%d,",q->value(2).toInt())+
	QString().sprintf("BROADCAST_SECURITY=%u,",q->value(3).toUInt())+
	QString().sprintf("HEARTBEAT_CART=%u,",q->value(4).toUInt())+
	QString().sprintf("HEARTBEAT_INTERVAL=%u,",q->value(5).toUInt())+
	"EDITOR_PATH=\""+RDEscapeString(q->value(6).toString())+"\","+
	QString().sprintf("FILTER_MODE=%d,",q->value(7).toInt())+
	"SYSTEM_MAINT=\""+RDEscapeString(q->value(8).toString())+"\","+
	"HTTP_STATION=\""+RDEscapeString(q->value(9).toString())+"\","+
	"CAE_STATION=\""+RDEscapeString(q->value(10).toString())+"\","+
	"BACKUP_DIR=\""+RDEscapeString(q->value(11).toString())+"\","+
	QString().sprintf("BACKUP_LIFE=%d,",q->value(12).toInt())+
	"START_JACK=\""+RDEscapeString(q->value(13).toString())+"\","+
	"JACK_SERVER_NAME=\""+RDEscapeString(q->value(14).toString())+"\","+
	"JACK_COMMAND_LINE=\""+RDEscapeString(q->value(15).toString())+"\","+
	QString().sprintf("CUE_CARD=%d,",q->value(16).toInt())+
	QString().sprintf("CUE_PORT=%d,",q->value(17).toInt())+
	QString().sprintf("CUE_START_CART=%u,",q->value(18).toInt())+
	QString().sprintf("CUE_STOP_CART=%u,",q->value(19).toInt())+
	QString().sprintf("CARTSLOT_COLUMNS=%d,",q->value(20).toInt())+
	QString().sprintf("CARTSLOT_ROWS=%d,",q->value(21).toInt())+
	"ENABLE_DRAGDROP=\""+RDEscapeString(q->value(22).toString())+"\","+
	"ENFORCE_PANEL_SETUP=\""+RDEscapeString(q->value(23).toString())+"\"";
      q1=new RDSqlQuery(sql);
      if(!q1->isActive()) {
	*err_msg=QObject::tr("host already exists");
	delete q1;
	delete q;
	return false;
      }
      delete q1;
    }
    else {
      *err_msg=QObject::tr("template host")+" \""+exemplar+"\" "+
	QObject::tr("does not exist");
      delete q;
      return false;
    }
    delete q;

    //
    // Clone Service Perms
    //
    sql=QString("select SERVICE_NAME from SERVICE_PERMS where ")+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into SERVICE_PERMS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	"SERVICE_NAME=\""+RDEscapeString(q->value(0).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDLibrary Config
    //
    sql=QString("select ")+
      "INPUT_CARD,"+           // 00
      "INPUT_PORT,"+           // 01
      "INPUT_TYPE,"+           // 02
      "OUTPUT_CARD,"+          // 03
      "OUTPUT_PORT,"+          // 04
      "VOX_THRESHOLD,"+        // 05
      "TRIM_THRESHOLD,"+       // 06
      "DEFAULT_FORMAT,"+       // 07
      "DEFAULT_CHANNELS,"+     // 08
      "DEFAULT_SAMPRATE,"+     // 09
      "DEFAULT_LAYER,"+        // 10
      "DEFAULT_BITRATE,"+      // 11
      "DEFAULT_RECORD_MODE,"+  // 12
      "DEFAULT_TRIM_STATE,"+   // 13
      "MAXLENGTH,"+            // 14
      "TAIL_PREROLL,"+         // 15
      "RIPPER_DEVICE,"+        // 16
      "PARANOIA_LEVEL,"+       // 17
      "RIPPER_LEVEL,"+         // 18
      "CDDB_SERVER,"+          // 19
      "READ_ISRC,"+            // 20
      "ENABLE_EDITOR,"+        // 21
      "SRC_CONVERTER,"+        // 22
      "LIMIT_SEARCH,"+         // 23
      "SEARCH_LIMITED "+       // 24
      "from RDLIBRARY where "+
      "(STATION=\""+RDEscapeString(exemplar)+"\")&&"+
      "(INSTANCE=0)";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into RDLIBRARY set ")+
	QString().sprintf("INPUT_CARD=%d,",q->value(0).toInt())+
	QString().sprintf("INPUT_PORT=%d,",q->value(1).toInt())+
	QString().sprintf("INPUT_TYPE=%d,",q->value(2).toInt())+
	QString().sprintf("OUTPUT_CARD=%d,",q->value(3).toInt())+
	QString().sprintf("OUTPUT_PORT=%d,",q->value(4).toInt())+
	QString().sprintf("VOX_THRESHOLD=%d,",q->value(5).toInt())+
	QString().sprintf("TRIM_THRESHOLD=%d,",q->value(6).toInt())+
	QString().sprintf("DEFAULT_FORMAT=%u,",q->value(7).toUInt())+
	QString().sprintf("DEFAULT_CHANNELS=%u,",q->value(8).toUInt())+
	QString().sprintf("DEFAULT_SAMPRATE=%u,",q->value(9).toUInt())+
	QString().sprintf("DEFAULT_LAYER=%u,",q->value(10).toUInt())+
	QString().sprintf("DEFAULT_BITRATE=%u,",q->value(11).toUInt())+
	QString().sprintf("DEFAULT_RECORD_MODE=%u,",q->value(12).toUInt())+
	"DEFAULT_TRIM_STATE=\""+RDEscapeString(q->value(13).toString())+"\","+
	QString().sprintf("MAXLENGTH=%d,",q->value(14).toInt())+
	QString().sprintf("TAIL_PREROLL=%u,",q->value(15).toUInt())+
	"RIPPER_DEVICE=\""+RDEscapeString(q->value(16).toString())+"\","+
	QString().sprintf("PARANOIA_LEVEL=%d,",q->value(17).toInt())+
	QString().sprintf("RIPPER_LEVEL=%d,",q->value(18).toInt())+
	"CDDB_SERVER=\""+RDEscapeString(q->value(19).toString())+"\","+
	"READ_ISRC=\""+RDEscapeString(q->value(20).toString())+"\","+
	"ENABLE_EDITOR=\""+RDEscapeString(q->value(21).toString())+"\","+
	QString().sprintf("SRC_CONVERTER=%d,",q->value(22).toInt())+
	QString().sprintf("LIMIT_SEARCH=%d,",q->value(23).toInt())+
	"SEARCH_LIMITED=\""+RDEscapeString(q->value(24).toString())+"\","+
	"STATION=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDLogEdit Config
    //
    sql=QString("select ")+
      "INPUT_CARD,"+          // 00
      "INPUT_PORT,"+          // 01
      "OUTPUT_CARD,"+         // 02
      "OUTPUT_PORT,"+         // 03
      "FORMAT,"+              // 04
      "DEFAULT_CHANNELS,"+    // 05
      "SAMPRATE,"+            // 06
      "LAYER,"+               // 07
      "BITRATE,"+             // 08
      "MAXLENGTH,"+           // 09
      "TAIL_PREROLL,"+        // 10
      "START_CART,"+          // 11
      "END_CART,"+            // 12
      "REC_START_CART,"+      // 13
      "REC_END_CART,"+        // 14
      "TRIM_THRESHOLD,"+      // 15
      "RIPPER_LEVEL,"+        // 16
      "DEFAULT_TRANS_TYPE "+  // 17
      "from RDLOGEDIT where "+
      "STATION=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into RDLOGEDIT set ")+
	QString().sprintf("INPUT_CARD=%d,",q->value(0).toInt())+
	QString().sprintf("INPUT_PORT=%d,",q->value(1).toInt())+
	QString().sprintf("OUTPUT_CARD=%d,",q->value(2).toInt())+
	QString().sprintf("OUTPUT_PORT=%d,",q->value(3).toInt())+
	QString().sprintf("FORMAT=%u,",q->value(4).toUInt())+
	QString().sprintf("DEFAULT_CHANNELS=%u,",q->value(5).toUInt())+
	QString().sprintf("SAMPRATE=%u,",q->value(6).toUInt())+
	QString().sprintf("LAYER=%u,",q->value(7).toUInt())+
	QString().sprintf("BITRATE=%u,",q->value(8).toUInt())+
	QString().sprintf("MAXLENGTH=%d,",q->value(9).toInt())+
	QString().sprintf("TAIL_PREROLL=%u,",q->value(10).toUInt())+
	"STATION=\""+RDEscapeString(name)+"\","+
	QString().sprintf("START_CART=%u,",q->value(11).toUInt())+
	QString().sprintf("END_CART=%u,",q->value(12).toUInt())+
	QString().sprintf("REC_START_CART=%u,",q->value(13).toUInt())+
	QString().sprintf("REC_END_CART=%u,",q->value(14).toUInt())+
	QString().sprintf("TRIM_THRESHOLD=%d,",q->value(15).toInt())+
	QString().sprintf("RIPPER_LEVEL=%d,",q->value(16).toInt())+
	QString().sprintf("DEFAULT_TRANS_TYPE=%d",q->value(17).toInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDCatch Config
    //
    sql=QString("select ")+
      "CHANNEL,"+             // 00
      "CARD_NUMBER,"+         // 01
      "PORT_NUMBER,"+         // 02
      "MON_PORT_NUMBER,"+     // 03
      "PORT_TYPE,"+           // 04
      "DEFAULT_FORMAT,"+      // 05
      "DEFAULT_CHANNELS,"+    // 06
      "DEFAULT_SAMPRATE,"+    // 07
      "DEFAULT_BITRATE,"+     // 08
      "DEFAULT_THRESHOLD,"+   // 09
      "DEFAULT_MONITOR_ON "+  // 10
      "from DECKS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into DECKS set ")+
	QString().sprintf("CHANNEL=%u,",q->value(0).toUInt())+
	QString().sprintf("CARD_NUMBER=%d,",q->value(1).toInt())+
	QString().sprintf("PORT_NUMBER=%d,",q->value(2).toInt())+
	QString().sprintf("MON_PORT_NUMBER=%d,",q->value(3).toInt())+
	"PORT_TYPE=\""+RDEscapeString(q->value(4).toString())+"\","+
	QString().sprintf("DEFAULT_FORMAT=%d,",q->value(5).toInt())+
	QString().sprintf("DEFAULT_CHANNELS=%d,",q->value(6).toInt())+
	QString().sprintf("DEFAULT_SAMPRATE=%d,",q->value(7).toInt())+
	QString().sprintf("DEFAULT_BITRATE=%d,",q->value(8).toInt())+
	QString().sprintf("DEFAULT_THRESHOLD=%d,",q->value(9).toInt())+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	"DEFAULT_MONITOR_ON=\""+RDEscapeString(q->value(10).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone Deck Events
    //
    sql=QString("select ")+
      "CHANNEL,"+      // 00
      "NUMBER,"+       // 01
      "CART_NUMBER "+  // 02
      "from DECK_EVENTS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into DECK_EVENTS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("CHANNEL=%u,",q->value(0).toUInt())+
	QString().sprintf("NUMBER=%d,",q->value(1).toInt())+
	QString().sprintf("CART_NUMBER=%u",q->value(2).toUInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDAirPlay Config
    //
    sql=QString("select ")+
      "SEGUE_LENGTH,"+           // 00
      "TRANS_LENGTH,"+           // 01
      "OP_MODE,"+                // 02
      "START_MODE,"+             // 03
      "PIE_COUNT_LENGTH,"+       // 04
      "PIE_COUNT_ENDPOINT,"+     // 05
      "CHECK_TIMESYNC,"+         // 06
      "STATION_PANELS,"+         // 07
      "USER_PANELS,"             // 08
      "SHOW_AUX_1,"+             // 09
      "SHOW_AUX_2,"+             // 10
      "CLEAR_FILTER,"+           // 11
      "DEFAULT_TRANS_TYPE,"+     // 12
      "BAR_ACTION,"+             // 13
      "FLASH_PANEL,"+            // 14
      "PAUSE_ENABLED,"+          // 15
      "UDP_ADDR0,"+              // 16
      "UDP_PORT0,"+              // 17
      "UDP_STRING0,"+            // 18
      "UDP_ADDR1,"+              // 19
      "UDP_PORT1,"+              // 20
      "UDP_STRING1,"+            // 21
      "UDP_ADDR2,"+              // 22
      "UDP_PORT2,"+              // 23
      "UDP_STRING2,"+            // 24
      "DEFAULT_SERVICE,"+        // 25
      "LOG_RML0,"+               // 26
      "LOG_RML1,"+               // 27
      "LOG_RML2,"+               // 28
      "BUTTON_LABEL_TEMPLATE,"+  // 29
      "EXIT_PASSWORD "+          // 30
      "from RDAIRPLAY where "+
      "STATION=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into RDAIRPLAY set ")+
	QString().sprintf("SEGUE_LENGTH=%d,",q->value(0).toInt())+
	QString().sprintf("TRANS_LENGTH=%d,",q->value(1).toInt())+
	QString().sprintf("OP_MODE=%d,",q->value(2).toInt())+
	QString().sprintf("START_MODE=%d,",q->value(3).toInt())+
	QString().sprintf("PIE_COUNT_LENGTH=%d,",q->value(4).toInt())+
	QString().sprintf("PIE_COUNT_ENDPOINT=%d,",q->value(5).toInt())+
	"CHECK_TIMESYNC=\""+RDEscapeString(q->value(6).toString())+"\","+
	QString().sprintf("STATION_PANELS=%d,",q->value(7).toInt())+
	QString().sprintf("USER_PANELS=%d,",q->value(8).toInt())+
	"SHOW_AUX_1=\""+RDEscapeString(q->value(9).toString())+"\","+
	"SHOW_AUX_2=\""+RDEscapeString(q->value(10).toString())+"\","+
	"CLEAR_FILTER=\""+RDEscapeString(q->value(11).toString())+"\","+
	QString().sprintf("DEFAULT_TRANS_TYPE=%u,",q->value(12).toUInt())+
	QString().sprintf("BAR_ACTION=%u,",q->value(13).toUInt())+
	"FLASH_PANEL=\""+RDEscapeString(q->value(14).toString())+"\","+
	"PAUSE_ENABLED=\""+RDEscapeString(q->value(15).toString())+"\","+
	"UDP_ADDR0=\""+RDEscapeString(q->value(16).toString())+"\","+
	QString().sprintf("UDP_PORT0=%u,",q->value(17).toUInt())+
	"UDP_STRING0=\""+RDEscapeString(q->value(18).toString())+"\","+
	"UDP_ADDR1=\""+RDEscapeString(q->value(19).toString())+"\","+
	QString().sprintf("UDP_PORT1=%u,",q->value(20).toUInt())+
	"UDP_STRING1=\""+RDEscapeString(q->value(21).toString())+"\","+
	"UDP_ADDR2=\""+RDEscapeString(q->value(22).toString())+"\","+
	QString().sprintf("UDP_PORT2=%u,",q->value(23).toUInt())+
	"UDP_STRING2=\""+RDEscapeString(q->value(24).toString())+"\","+
	"STATION=\""+RDEscapeString(name)+"\","+
	"DEFAULT_SERVICE=\""+RDEscapeString(q->value(25).toString())+"\","+
	"LOG_RML0=\""+RDEscapeString(q->value(26).toString())+"\","+
	"LOG_RML1=\""+RDEscapeString(q->value(27).toString())+"\","+
	"LOG_RML2=\""+RDEscapeString(q->value(28).toString())+"\","+
	"BUTTON_LABEL_TEMPLATE=\""+RDEscapeString(q->value(29).toString())+
	"\","+
	"EXIT_PASSWORD=\""+RDEscapeString(q->value(30).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    sql=QString("select ")+
      "INSTANCE,"+          // 00
      "CARD,"+              // 01
      "PORT,"+              // 02
      "START_RML,"+         // 03
      "STOP_RML,"+          // 04
      "START_GPI_MATRIX,"+  // 05
      "START_GPI_LINE,"+    // 06
      "START_GPO_MATRIX,"+  // 07
      "START_GPO_LINE,"+    // 08
      "STOP_GPI_MATRIX,"+   // 09
      "STOP_GPI_LINE,"+     // 10
      "STOP_GPO_MATRIX,"+   // 11
      "STOP_GPO_LINE "+     // 12
      "from RDAIRPLAY_CHANNELS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";

    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into RDAIRPLAY_CHANNELS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("INSTANCE=%u,",q->value(0).toUInt())+
	QString().sprintf("CARD=%d,",q->value(1).toInt())+
	QString().sprintf("PORT=%d,",q->value(2).toInt())+
	"START_RML=\""+RDEscapeString(q->value(3).toString())+"\","+
	"STOP_RML=\""+RDEscapeString(q->value(4).toString())+"\","+
	QString().sprintf("START_GPI_MATRIX=%d,",q->value(5).toInt())+
	QString().sprintf("START_GPI_LINE=%d,",q->value(6).toInt())+
	QString().sprintf("START_GPO_MATRIX=%d,",q->value(7).toInt())+
	QString().sprintf("START_GPO_LINE=%d,",q->value(8).toInt())+
	QString().sprintf("STOP_GPI_MATRIX=%d,",q->value(9).toInt())+
	QString().sprintf("STOP_GPI_LINE=%d,",q->value(10).toInt())+
	QString().sprintf("STOP_GPO_MATRIX=%d,",q->value(11).toInt())+
	QString().sprintf("STOP_GPO_LINE=%d",q->value(12).toInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDAirPlay Log Modes
    //
    sql=QString("select ")+
      "MACHINE,"+     // 00
      "START_MODE,"+  // 01
      "OP_MODE from LOG_MODES where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("insert into LOG_MODES set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("MACHINE=%d,",q->value(0).toInt())+
	QString().sprintf("START_MODE=%d,",q->value(1).toInt())+
	QString().sprintf("OP_MODE=%d",q->value(2).toInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RDPanel Config
    //
    sql=QString("select ")+
      "STATION_PANELS,"+         // 00
      "USER_PANELS,"+            // 01
      "CLEAR_FILTER,"+           // 02
      "FLASH_PANEL,"+            // 03
      "DEFAULT_SERVICE,"+        // 04
      "BUTTON_LABEL_TEMPLATE "+  // 05
      "from RDPANEL where "+
      "STATION=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString("insert into RDPANEL set ")+
	QString().sprintf("STATION_PANELS=%d,",q->value(0).toInt())+
	QString().sprintf("USER_PANELS=%d,",q->value(1).toInt())+
	"CLEAR_FILTER=\""+RDEscapeString(q->value(2).toString())+"\","+
	"FLASH_PANEL=\""+RDEscapeString(q->value(3).toString())+"\","+
	"STATION=\""+RDEscapeString(name)+"\","+
	"DEFAULT_SERVICE=\""+RDEscapeString(q->value(4).toString())+"\","+
	"BUTTON_LABEL_TEMPLATE=\""+RDEscapeString(q->value(5).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    sql=QString("select ")+
      "INSTANCE,"+
      "CARD,"+
      "PORT,"+
      "START_RML,"+
      "STOP_RML,"+
      "START_GPI_MATRIX,"+
      "START_GPI_LINE,"+
      "START_GPO_MATRIX,"+
      "START_GPO_LINE,"+
      "STOP_GPI_MATRIX,"+
      "STOP_GPI_LINE,"+
      "STOP_GPO_MATRIX,"+
      "STOP_GPO_LINE "+
      "from RDPANEL_CHANNELS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into RDPANEL_CHANNELS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("INSTANCE=%u,",q->value(0).toUInt())+
	QString().sprintf("CARD=%d,",q->value(1).toInt())+
	QString().sprintf("PORT=%d,",q->value(2).toInt())+
	"START_RML=\""+RDEscapeString(q->value(3).toString())+"\","+
	"STOP_RML=\""+RDEscapeString(q->value(4).toString())+"\","+
	QString().sprintf("START_GPI_MATRIX=%d,",q->value(5).toInt())+
	QString().sprintf("START_GPI_LINE=%d,",q->value(6).toInt())+
	QString().sprintf("START_GPO_MATRIX=%d,",q->value(7).toInt())+
	QString().sprintf("START_GPO_LINE=%d,",q->value(8).toInt())+
	QString().sprintf("STOP_GPI_MATRIX=%d,",q->value(9).toInt())+
	QString().sprintf("STOP_GPI_LINE=%d,",q->value(10).toInt())+
	QString().sprintf("STOP_GPO_MATRIX=%d,",q->value(11).toInt())+
	QString().sprintf("STOP_GPO_LINE=%d",q->value(12).toInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone Audio Port Settings
    //
    sql=QString("select ")+
      "CARD_NUMBER,"    // 00
      "CLOCK_SOURCE,";  // 01
    for(int i=0;i<RD_MAX_PORTS;i++) {
      sql+=QString().sprintf("INPUT_%d_LEVEL,",i)+
	QString().sprintf("INPUT_%d_MODE,",i)+
	QString().sprintf("INPUT_%d_TYPE,",i)+
	QString().sprintf("OUTPUT_%d_LEVEL,",i);
    }
    sql=sql.left(sql.length()-1);
    sql+=QString(" from AUDIO_PORTS where ")+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into AUDIO_PORTS set ")+
	QString().sprintf("CARD_NUMBER=%u,",q->value(0).toUInt())+
	QString().sprintf("CLOCK_SOURCE=%d,",q->value(1).toInt())+
	"STATION_NAME=\""+RDEscapeString(name)+"\",";
      for(int i=0;i<RD_MAX_PORTS;i++) {
	sql+=QString().sprintf("INPUT_%d_LEVEL=%d,INPUT_%d_MODE=%d,\
                                INPUT_%d_TYPE=%d,OUTPUT_%d_LEVEL=%d,",
			       i,q->value(2+3*i).toInt(),
			       i,q->value(3+3*i).toInt(),
			       i,q->value(4+3*i).toInt(),
			       i,q->value(5+3*i).toInt());
      }
      sql=sql.left(sql.length()-1);
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone the Serial Setups
    //
    sql=QString("select ")+
      "PORT_ID,"+      // 00
      "ACTIVE,"+       // 01
      "PORT,"+         // 02
      "BAUD_RATE,"+    // 03
      "DATA_BITS,"+    // 04
      "STOP_BITS,"+    // 05
      "PARITY,"+       // 06
      "TERMINATION "+  // 07
      "from TTYS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
     q=new RDSqlQuery(sql);
     while(q->next()) {
       sql=QString("insert into TTYS set ")+
	 QString().sprintf("PORT_ID=%u,",q->value(0).toUInt())+
	 "ACTIVE=\""+RDEscapeString(q->value(1).toString())+"\","+
	 "PORT=\""+RDEscapeString(q->value(2).toString())+"\","+
	 QString().sprintf("BAUD_RATE=%d,",q->value(3).toInt())+
	 QString().sprintf("DATA_BITS=%d,",q->value(4).toInt())+
	 QString().sprintf("STOP_BITS=%d,",q->value(5).toInt())+
	 QString().sprintf("PARITY=%d,",q->value(6).toInt())+
	 QString().sprintf("TERMINATION=%d,",q->value(7).toInt())+
	 "STATION_NAME=\""+RDEscapeString(name)+"\"";
       q1=new RDSqlQuery(sql);
       delete q1;
     }
     delete q;

     //
     // Clone Matrices
     //
     sql=QString("select ")+
       "NAME,"+          // 00
       "MATRIX,"+        // 01
       "TYPE,"+          // 02
       "LAYER,"+         // 03
       "PORT_TYPE,"+     // 04
       "PORT_TYPE_2,"+   // 05
       "CARD,"+          // 06
       "PORT,"+          // 07
       "PORT_2,"+        // 08
       "IP_ADDRESS,"+    // 09
       "IP_ADDRESS_2,"+  // 10
       "IP_PORT,"+       // 11
       "IP_PORT_2,"+     // 12
       "USERNAME,"+      // 13
       "USERNAME_2,"+    // 14
       "PASSWORD,"+      // 15
       "PASSWORD_2,"+    // 16
       "START_CART,"+    // 17
       "STOP_CART,"+     // 18
       "START_CART_2,"+  // 19
       "STOP_CART_2,"+   // 20
       "GPIO_DEVICE,"+   // 21
       "INPUTS,"+        // 22
       "OUTPUTS,"+       // 23
       "GPIS,"+          // 24
       "GPOS,"+          // 25
       "FADERS,"+        // 26
       "DISPLAYS "+      // 27
       "from MATRICES where "+
       "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
     q=new RDSqlQuery(sql);
     while(q->next()) {
       sql=QString("insert into MATRICES set ")+
	 "STATION_NAME=\""+RDEscapeString(name)+"\","+
	 "NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	 QString().sprintf("MATRIX=%d,",q->value(1).toInt())+
	 QString().sprintf("TYPE=%d,",q->value(2).toInt())+
	 QString().sprintf("LAYER=%d,",q->value(3).toInt())+
	 QString().sprintf("PORT_TYPE=%d,",q->value(4).toInt())+
	 QString().sprintf("PORT_TYPE_2=%d,",q->value(5).toInt())+
	 QString().sprintf("CARD=%d,",q->value(6).toInt())+
	 QString().sprintf("PORT=%d,",q->value(7).toInt())+
	 QString().sprintf("PORT_2=%d,",q->value(8).toInt())+
	 "IP_ADDRESS=\""+RDEscapeString(q->value(9).toString())+"\","+
	 "IP_ADDRESS_2=\""+RDEscapeString(q->value(10).toString())+"\","+
	 QString().sprintf("IP_PORT=%d,",q->value(11).toInt())+
	 QString().sprintf("IP_PORT_2=%d,",q->value(12).toInt())+
	 "USERNAME=\""+RDEscapeString(q->value(13).toString())+"\","+
	 "USERNAME_2=\""+RDEscapeString(q->value(14).toString())+"\","+
	 "PASSWORD=\""+RDEscapeString(q->value(15).toString())+"\","+
	 "PASSWORD_2=\""+RDEscapeString(q->value(16).toString())+"\","+
	 "START_CART=\""+RDEscapeString(q->value(17).toString())+"\","+
	 "STOP_CART=\""+RDEscapeString(q->value(18).toString())+"\","+
	 "START_CART_2=\""+RDEscapeString(q->value(19).toString())+"\","+
	 "STOP_CART_2=\""+RDEscapeString(q->value(20).toString())+"\","+
	 "GPIO_DEVICE=\""+RDEscapeString(q->value(21).toString())+"\","+
	 QString().sprintf("INPUTS=%d,",q->value(22).toInt())+
	 QString().sprintf("OUTPUTS=%d,",q->value(23).toInt())+
	 QString().sprintf("GPIS=%d,",q->value(24).toInt())+
	 QString().sprintf("GPOS=%d,",q->value(25).toInt())+
	 QString().sprintf("FADERS=%d,",q->value(26).toInt())+
	 QString().sprintf("DISPLAYS=%d ",q->value(27).toInt());
       q1=new RDSqlQuery(sql);
       delete q1;
     }
     delete q;

     //
     // Clone Matrix Inputs
     //
     sql=QString("select ")+
       "MATRIX,"+        // 00
       "NUMBER,"+        // 01
       "NAME,"+          // 02
       "FEED_NAME,"+     // 03
       "CHANNEL_MODE,"+  // 04
       "ENGINE_NUM,"+    // 05
       "DEVICE_NUM "+    // 06
       "from INPUTS where "+
       "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
     q=new RDSqlQuery(sql);
     while(q->next()) {
       sql=QString("insert into INPUTS set ")+
	 QString().sprintf("MATRIX=%d,",q->value(0).toInt())+
	 QString().sprintf("NUMBER=%d,",q->value(1).toInt())+
	 "NAME=\""+RDEscapeString(q->value(2).toString())+"\","+
	 "FEED_NAME=\""+RDEscapeString(q->value(3).toString())+"\","+
	 QString().sprintf("CHANNEL_MODE=%d,",q->value(4).toInt())+
	 QString().sprintf("ENGINE_NUM=%d,",q->value(5).toInt())+
	 QString().sprintf("DEVICE_NUM=%d,",q->value(6).toInt())+
	 "STATION_NAME=\""+RDEscapeString(name)+"\"";
       q1=new RDSqlQuery(sql);
       delete q1;
     }
     delete q;

     //
     // Clone Matrix Outputs
     //
     sql=QString("select ")+
       "MATRIX,"+      // 00
       "NUMBER,"+      // 01
       "NAME,"+        // 02
       "ENGINE_NUM,"+  // 03
       "DEVICE_NUM "+  // 04
       "from OUTPUTS where "+
       "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
     q=new RDSqlQuery(sql);
     while(q->next()) {
       sql=QString("insert into OUTPUTS set ")+
	 QString().sprintf("MATRIX=%d,",q->value(0).toInt())+
	 QString().sprintf("NUMBER=%d,",q->value(1).toInt())+
	 "NAME=\""+RDEscapeString(q->value(2).toString())+"\","+
	 QString().sprintf("ENGINE_NUM=%d,",q->value(3).toInt())+
	 QString().sprintf("DEVICE_NUM=%d,",q->value(4).toInt())+
	 "STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone GPIs
    //
    sql=QString("select ")+
      "MATRIX,"+      // 00
      "NUMBER,"+      // 01
      "MACRO_CART "+  // 02
      "from GPIS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into GPIS set ")+
	QString().sprintf("MATRIX=%d,",q->value(0).toInt())+
	QString().sprintf("NUMBER=%d,",q->value(1).toInt())+
	QString().sprintf("MACRO_CART=%d,",q->value(2).toInt())+
	"STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Close vGuest Settings
    //
    sql=QString("select ")+
      "MATRIX_NUM,"+
      "VGUEST_TYPE,"+
      "NUMBER,"+
      "ENGINE_NUM,"+
      "DEVICE_NUM,"+
      "SURFACE_NUM,"+
      "RELAY_NUM,"+
      "BUSS_NUM "+
      "from VGUEST_RESOURCES where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into VGUEST_RESOURCES set ")+
	QString().sprintf("MATRIX_NUM=%d,",q->value(0).toInt())+
	QString().sprintf("VGUEST_TYPE=%d,",q->value(1).toInt())+
	QString().sprintf("NUMBER=%d,",q->value(2).toInt())+
	QString().sprintf("ENGINE_NUM=%d,",q->value(3).toInt())+
	QString().sprintf("DEVICE_NUM=%d,",q->value(4).toInt())+
	QString().sprintf("SURFACE_NUM=%d,",q->value(5).toInt())+
	QString().sprintf("RELAY_NUM=%d,",q->value(6).toInt())+
	QString().sprintf("BUSS_NUM=%d,",q->value(7).toInt())+
	"STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone Host Variables
    //
    sql=QString("select ")+
      "NAME,"+
      "VARVALUE,"+
      "REMARK "+
      "from HOSTVARS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into HOSTVARS set ")+
	"NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	"VARVALUE=\""+RDEscapeString(q->value(1).toString())+"\","+
	"REMARK=\""+RDEscapeString(q->value(2).toString())+"\","+
	"STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone System Panels
    //
    sql=QString("select ")+
      "PANEL_NO,"+
      "ROW_NO,"+
      "COLUMN_NO,"+
      "LABEL,"+
      "CART,"+
      "DEFAULT_COLOR "+
      "from PANELS where "+
      QString().sprintf("(TYPE=%d)&&",RDAirPlayConf::StationPanel)+
      "(OWNER=\""+RDEscapeString(exemplar)+"\")";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into PANELS set ")+
	QString().sprintf("PANEL_NO=%d,",q->value(0).toInt())+
	QString().sprintf("ROW_NO=%d,",q->value(1).toInt())+
	QString().sprintf("COLUMN_NO=%d,",q->value(2).toInt())+
	"LABEL=\""+RDEscapeString(q->value(3).toString())+"\","+
	QString().sprintf("CART=%u,",q->value(4).toUInt())+
	"DEFAULT_COLOR=\""+RDEscapeString(q->value(5).toString())+"\","+
	QString().sprintf("TYPE=%d,",RDAirPlayConf::StationPanel)+
	"OWNER=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    sql=QString("select ")+
      "PANEL_NO,"+       // 00
      "ROW_NO,"+         // 01
      "COLUMN_NO,"+      // 02
      "LABEL,"+          // 03
      "CART,"+           // 04
      "DEFAULT_COLOR "+  // 05
      "from EXTENDED_PANELS where "+
      QString().sprintf("(TYPE=%d)&&",RDAirPlayConf::StationPanel)+
      "(OWNER=\""+RDEscapeString(exemplar)+"\")";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into EXTENDED_PANELS set ")+
	QString().sprintf("PANEL_NO=%d,",q->value(0).toInt())+
	QString().sprintf("ROW_NO=%d,",q->value(1).toInt())+
	QString().sprintf("COLUMN_NO=%d,",q->value(2).toInt())+
	"LABEL=\""+RDEscapeString(q->value(3).toString())+"\","+
	QString().sprintf("CART=%u,",q->value(4).toUInt())+
	"DEFAULT_COLOR=\""+RDEscapeString(q->value(5).toString())+"\","+
	QString().sprintf("TYPE=%d,",RDAirPlayConf::StationPanel)+
	"OWNER=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone Hotkeys
    //
    sql=QString("select ")+
      "MODULE_NAME,"+  // 00
      "KEY_ID,"+       // 01
      "KEY_VALUE,"+    // 02
      "KEY_LABEL "+    // 03
      "from RDHOTKEYS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into RDHOTKEYS set ")+
	"MODULE_NAME=\""+RDEscapeString(q->value(0).toString())+"\","+
	QString().sprintf("KEY_ID=%d,",q->value(1).toInt())+
	"KEY_VALUE=\""+RDEscapeString(q->value(2).toString())+"\","+	\
	"KEY_LABEL=\""+RDEscapeString(q->value(3).toString())+"\","+
	"STATION_NAME=\""+RDEscapeString(name)+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone RLM Parameters
    //
    sql=QString("select ")+
      "LOG_MACHINE,"+  // 00
      "PLUGIN_PATH,"+  // 01
      "PLUGIN_ARG "+   // 02
      "from NOWNEXT_PLUGINS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into NOWNEXT_PLUGINS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("LOG_MACHINE=%u,",q->value(0).toUInt())+
	"PLUGIN_PATH=\""+RDEscapeString(q->value(1).toString())+"\","+
	"PLUGIN_ARG=\""+RDEscapeString(q->value(2).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone CARTSLOTS
    //
    sql=QString("select ")+
      "SLOT_NUMBER,"+          // 00
      "MODE,"+                 // 01
      "DEFAULT_MODE,"+         // 02
      "STOP_ACTION,"+          // 03
      "DEFAULT_STOP_ACTION,"+  // 04
      "CART_NUMBER,"+          // 05
      "DEFAULT_CART_NUMBER,"+  // 06
      "HOOK_MODE,"+            // 07
      "DEFAULT_HOOK_MODE,"+    // 08
      "SERVICE_NAME,"+         // 09
      "CARD,"+                 // 10
      "INPUT_PORT,"+           // 11
      "OUTPUT_PORT "+          // 12
      "from CARTSLOTS where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into CARTSLOTS set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("SLOT_NUMBER=%u,",q->value(0).toUInt())+
	QString().sprintf("MODE=%d,",q->value(1).toInt())+
	QString().sprintf("DEFAULT_MODE=%d,",q->value(2).toInt())+
	QString().sprintf("STOP_ACTION=%d,",q->value(3).toInt())+
	QString().sprintf("DEFAULT_STOP_ACTION=%d,",q->value(4).toInt())+
	QString().sprintf("CART_NUMBER=%d,",q->value(5).toInt())+
	QString().sprintf("DEFAULT_CART_NUMBER=%d,",q->value(6).toInt())+
	QString().sprintf("HOOK_MODE=%d,",q->value(7).toInt())+
	QString().sprintf("DEFAULT_HOOK_MODE=%d,",q->value(8).toInt())+
	"SERVICE_NAME=\""+RDEscapeString(q->value(9).toString())+"\","+
	QString().sprintf("CARD=%d,",q->value(10).toInt())+
	QString().sprintf("INPUT_PORT=%d,",q->value(11).toInt())+
	QString().sprintf("OUTPUT_PORT=%d",q->value(12).toInt());
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Clone Livewire Nodes
    //
    sql=QString("select ")+
      "MATRIX,"+       // 00
      "BASE_OUTPUT,"+  // 01
      "HOSTNAME,"+     // 02
      "PASSWORD,"+     // 03
      "TCP_PORT,"+     // 04
      "DESCRIPTION "+  // 05
      "from SWITCHER_NODES where "+
      "STATION_NAME=\""+RDEscapeString(exemplar)+"\"";
    q=new RDSqlQuery(sql);
    while(q->next()) {
      sql=QString("insert into SWITCHER_NODES set ")+
	"STATION_NAME=\""+RDEscapeString(name)+"\","+
	QString().sprintf("MATRIX=%d,",q->value(0).toInt())+
	QString().sprintf("BASE_OUTPUT=%d,",q->value(1).toInt())+
	"HOSTNAME=\""+RDEscapeString(q->value(2).toString())+"\","+
	"PASSWORD=\""+RDEscapeString(q->value(3).toString())+"\","+
	QString().sprintf("TCP_PORT=%d,",q->value(4).toInt())+
	"DESCRIPTION=\""+RDEscapeString(q->value(5).toString())+"\"";
      q1=new RDSqlQuery(sql);
      delete q1;
    }
    delete q;
  }
  return true;
}


void RDStation::remove(const QString &name)
{
  QString sql;
  RDSqlQuery *q;
  RDSqlQuery *q1;

  sql=QString().sprintf("delete from DECKS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString("delete from DECK_EVENTS where ")+
    "STATION_NAME=\""+RDEscapeString(name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from TTYS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from AUDIO_PORTS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RECORDINGS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=
    QString().sprintf("delete from SERVICE_PERMS where STATION_NAME=\"%s\"",
		      (const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDAIRPLAY where STATION=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDAIRPLAY_CHANNELS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDPANEL where STATION=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDPANEL_CHANNELS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDLOGEDIT where STATION=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from MATRICES where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from INPUTS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from OUTPUTS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from VGUEST_RESOURCES where\
                           STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDLIBRARY where STATION=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from GPIS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from HOSTVARS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from STATIONS where NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=
    QString().sprintf("delete from REPORT_STATIONS where STATION_NAME=\"%s\"",
		      (const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from PANELS where (TYPE=%d && OWNER=\"%s\")",
			RDAirPlayConf::StationPanel,
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().
    sprintf("delete from EXTENDED_PANELS where (TYPE=%d && OWNER=\"%s\")",
	    RDAirPlayConf::StationPanel,(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;

  sql=QString().sprintf("select ID from ENCODERS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("delete from ENCODER_CHANNELS where ENCODER_ID=%d",
			  q->value(0).toInt());
    q1=new RDSqlQuery(sql);
    delete q1;
    sql=QString().sprintf("delete from ENCODER_SAMPLERATES where ENCODER_ID=%d",
			  q->value(0).toInt());
    q1=new RDSqlQuery(sql);
    delete q1;
    sql=QString().sprintf("delete from ENCODER_BITRATES where ENCODER_ID=%d",
			  q->value(0).toInt());
    q1=new RDSqlQuery(sql);
    delete q1;
  }
  delete q;
  sql=QString().sprintf("delete from ENCODERS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from RDHOTKEYS where STATION_NAME=\"%s\"",
			(const char *)RDEscapeString(name));
  q=new RDSqlQuery(sql);
  delete q;

  sql=QString("delete from LOG_MODES where ")+
    "STATION_NAME=\""+RDEscapeString(name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;

  sql=QString("delete from NOWNEXT_PLUGINS where ")+
    "STATION_NAME=\""+RDEscapeString(name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;

  sql=QString("delete from CARTSLOTS where ")+
    "STATION_NAME=\""+RDEscapeString(name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;

  sql=QString("delete from SWITCHER_NODES where ")+
    "STATION_NAME=\""+RDEscapeString(name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;
}


void RDStation::SetRow(const QString &param,const QString &value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE STATIONS SET %s=\"%s\" WHERE NAME=\"%s\"",
			(const char *)param,
			(const char *)RDEscapeString(value),
			(const char *)RDEscapeString(station_name));
  q=new RDSqlQuery(sql);
  delete q;
}


void RDStation::SetRow(const QString &param,int value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE STATIONS SET %s=%d WHERE NAME=\"%s\"",
			(const char *)param,
			value,
			(const char *)RDEscapeString(station_name));
  q=new RDSqlQuery(sql);
  delete q;
}


void RDStation::SetRow(const QString &param,unsigned value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE STATIONS SET %s=%u WHERE NAME=\"%s\"",
			(const char *)param,
			value,
			(const char *)RDEscapeString(station_name));
  q=new RDSqlQuery(sql);
  delete q;
}


void RDStation::SetRow(const QString &param,bool value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE STATIONS SET %s=\"%s\" WHERE NAME=\"%s\"",
			(const char *)param,
			(const char *)RDYesNo(value),
			(const char *)RDEscapeString(station_name));
  q=new RDSqlQuery(sql);
  delete q;
}
