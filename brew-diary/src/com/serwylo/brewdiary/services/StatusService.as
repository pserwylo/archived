/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 8/01/11
 * Time: 8:19 PM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.services
{
import com.serwylo.brewdiary.data.Status;

import flash.data.SQLResult;
import flash.data.SQLStatement;

public class StatusService
{

	private static var _statusList:Array;

	public static function getStatusList():Array
	{
		if ( _statusList == null )
		{
			var sql:SQLStatement = new SQLStatement();
			sql.sqlConnection = Database.instance.connection;
			sql.text = "SELECT statusSerial, statusLabel FROM status;";
			sql.execute();

			_statusList = new Array();
			var result:SQLResult = sql.getResult();
			for each ( var s:Object in result.data )
			{
				var status:Status = new Status();
				status.statusLabel = s.statusLabel;
				status.statusSerial = s.statusSerial;
				_statusList.push( status );
			}
		}
		return _statusList;
	}

	/**
	 * Doesn't connect to the database, rather it will loop through the status list.
	 * This is because the status list pretty much always exists, and it will be nice to have a reference
	 * to the same object everywhere rather than creating a new status object.
	 * @param statusSerial
	 * @return
	 */
	public static function getStatusBySerial( statusSerial:Number ):Status
	{
		for each ( var s:Status in getStatusList() )
		{
			if ( s.statusSerial == statusSerial )
			{
				return s;
			}
		}
		return null;
	}
}
}
