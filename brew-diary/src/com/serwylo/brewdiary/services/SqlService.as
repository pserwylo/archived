/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 8/01/11
 * Time: 10:46 AM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.services
{
import flash.data.SQLConnection;
import flash.data.SQLResult;
import flash.data.SQLStatement;
import flash.errors.SQLError;
import flash.events.EventDispatcher;

import mx.controls.Alert;

public class SqlService extends EventDispatcher
{

	protected static function query( sql:String, params:Object = null ):SQLStatement
	{
		var statement:SQLStatement = new SQLStatement();
		statement.text = sql;
		statement.sqlConnection = Database.instance.connection;
		if ( params )
		{
			for ( var label:String in params )
			{
				var newLabel:String = label.substr( 0, 1 ) == ":" ? label : ":" + label;
				statement.parameters[ newLabel ] = params[ label ];
			}
		}

		try
		{
			statement.execute();
		}
		catch ( error:SQLError )
		{
			Alert.show( "SQL Error " + error.detailID + ": " + error.details );
		}
		catch ( error:Error )
		{
			Alert.show( error.type + " error during query " + error.errorID + ": " + error.message );
		}
		return statement;
	}

	public function SqlService()
	{
	}

	public static function lastInsertId():Number
	{
		var query:String = "SELECT last_insert_rowid() as id;";
		var sql:SQLStatement = SqlService.query( query );

		var result:SQLResult = sql.getResult();
		var data:Object = result.data.push();
		if ( data )
		{
			return Number( data );
		}
		else
		{
			return -1;
		}
	}
}

}
