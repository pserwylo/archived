package com.serwylo.brewdiary.services
{

import com.serwylo.brewdiary.data.Brew;

import com.serwylo.brewdiary.data.Status;

import flash.data.SQLConnection;
import flash.data.SQLResult;
import flash.data.SQLStatement;

import mx.controls.Alert;
import mx.formatters.DateFormatter;

public class BrewService extends SqlService
{

	public function BrewService()
	{
	}

	public static function getBrewList():Array
	{
		var sql:SQLStatement = new SQLStatement();
		sql.sqlConnection = Database.instance.connection;
		sql.text = "SELECT brewSerial, brewLabel, startDate, initialFermentationDate, bottledDate, statusSerial FROM brew;";
		sql.execute();

		var brewList:Array = new Array();
		var result:SQLResult = sql.getResult();
		for each ( var b:Object in result.data )
		{
			var brew:Brew = new Brew();
			brew.brewSerial = b.brewSerial;
			brew.brewLabel = b.brewLabel;
			brew.startDate = b.startDate;
			brew.status = StatusService.getStatusBySerial( b.statusSerial );
			brewList.push( brew );
		}

		return brewList;
	}

	public static function saveBrewService( brew:Brew ):void
	{

		var fields:String = "brewLabel";
		var values:String = ":brewLabel";
		var params:Object = { brewLabel : brew.brewLabel }

		if ( brew.brewSerial > 0 )
		{
			fields += ", brewSerial";
			values += ", :brewSerial";
			params.brewSerial = brew.brewSerial;
		}

		if ( brew.startDate )
		{
			var df:DateFormatter = new DateFormatter();
			df.formatString = "YYYY/MM/DD HH:MM::SS";
			fields += ", startDate";
			values += ", :startDate";
			params.startDate = brew.startDate;
		}

		if ( brew.status )
		{
			fields += ", statusSerial";
			values += ", :statusSerial";
			params.statusSerial = brew.status.statusSerial;
		}
		
		var query:String = "REPLACE INTO brew ( " + fields + " ) VALUES (" + values + ")";
		SqlService.query( query, params );
		if ( isNaN( brew.brewSerial ) )
		{
			brew.brewSerial = SqlService.lastInsertId();
		}

	}

}

}
