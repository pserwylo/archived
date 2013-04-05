/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 8/01/11
 * Time: 10:34 AM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.services
{
import flash.data.SQLConnection;
import flash.data.SQLStatement;
import flash.errors.SQLError;
import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.SQLErrorEvent;
import flash.events.SQLEvent;
import flash.filesystem.File;

import mx.controls.Alert;

public class Database extends EventDispatcher
{

	private static var _instance:Database;
	public static function get instance():Database
	{
		if ( _instance == null )
		{
			_instance = new Database();
		}
		return _instance;
	}

	private var _connection:SQLConnection;
	public function get connection():SQLConnection
	{
		if ( _connection == null )
		{
			throw new Error( "Database not yet initialized. Please call SqlService.init() first." );
		}
		return _connection;
	}

	public function init():void
	{
		if ( _connection != null )
		{
			throw new Error( "Database already initialized." );
		}
		
		var file:File = File.applicationStorageDirectory.resolvePath( "brews11.db" );
		// Alert.show( file.nativePath );
		_connection = new SQLConnection();
		_connection.addEventListener( SQLEvent.OPEN, this.onDatabaseOpen );
		_connection.addEventListener( SQLErrorEvent.ERROR, this.dispatchEvent );
		_connection.open( file );
	}

	public function Database()
	{
	}

	private function onDatabaseOpen( event:SQLEvent ):void
	{
		this.initBrewTable();
		this.initNoteTypeTable();
		this.initNoteTable();
		this.initReview();
		this.initSgReadingTable();
		this.initStatusTable();
		this.initBottleTypeTable();
		this.initBottleTable();
		this.dispatchEvent( new SQLEvent( SQLEvent.OPEN ) );
	}

	private function initBrewTable():void
	{
		if ( !this.doesTableExists( 'brew' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE brew (" +
					"brewSerial INTEGER PRIMARY KEY, " +
					"startDate DATE, " +
					"initialFermentationDate DATE, " +
					"bottledDate DATE, " +
					"statusSerial NUMERIC, " +
					"brewLabel TEXT" +
				");";
			ddl.execute();
		}
	}

	private function initNoteTypeTable():void
	{
		if ( !this.doesTableExists( 'noteType' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE noteType (" +
					"noteTypeSerial INTEGER PRIMARY KEY, " +
					"noteTypeLabel TEXT" +
				");";
			ddl.execute();

			var notes:Array = [
				{ serial : 1, label : 'Note' },
				{ serial : 2, label : 'Error' },
				{ serial : 3, label : 'Idea' }
			];
			
			for each ( var note:Object in notes )
			{
				var sql:SQLStatement = new SQLStatement();
				sql.sqlConnection = _connection;
				sql.text = "INSERT INTO noteType (noteTypeSerial,noteTypeLabel) VALUES (" + note.serial + ",'" + note.label + "')";
				sql.execute();
			}
		}
	}

	private function initNoteTable():void
	{
		if ( !this.doesTableExists( 'note' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE note (" +
					"noteSerial INTEGER PRIMARY KEY, " +
					"brewSerial NUMERIC, " +
					"noteTypeSerial NUMERIC,  " +
					"message TEXT, " +
					"createdDate DATE" +
				");";
			ddl.execute();
		}
	}

	private function initReview():void
	{
		if ( !this.doesTableExists( 'review' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE review (" +
					"reviewSerial INTEGER PRIMARY KEY, " +
					"brewSerial NUMERIC, " +
					"createdDate DATE, " +
					"shortDescription TEXT, " +
					"longDescription TEXT, " +
					"rating NUMERIC" +
				");";
			ddl.execute();
		}
	}

	private function initSgReadingTable():void
	{
		if ( !this.doesTableExists( 'sgReading' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE sgReading (" +
					"sgReadingSerial INTEGER PRIMARY KEY, " +
					"brewSerial NUMERIC, " +
					"sgValue NUMERIC, " +
					"readingDate DATE" +
				");";
			ddl.execute();
		}
	}

	private function initStatusTable():void
	{
		if ( !this.doesTableExists( 'status' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE status (" +
					"statusSerial INTEGER PRIMARY KEY, " +
					"statusLabel TEXT" +
				");";
			ddl.execute();

			var statuses:Array = [
				{ serial : 1, label : 'Not Started' },
				{ serial : 2, label : 'Initial Fermentation' },
				{ serial : 3, label : 'Bottle Fermentation' },
				{ serial : 4, label : 'Bottled/Ageing' },
				{ serial : 5, label : 'Ready to Drink' },
				{ serial : 6, label : 'Empty' }
			];

			for each ( var status:Object in statuses )
			{
				var sql:SQLStatement = new SQLStatement();
				sql.sqlConnection = _connection;
				sql.text = "INSERT INTO status (statusSerial,statusLabel) VALUES (" + status.serial + ",'" + status.label + "')";
				sql.execute();
			}
		}
	}

	private function initBottleTypeTable():void
	{
		if ( !this.doesTableExists( 'bottleType' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE bottleType (" +
					"bottleTypeSerial INTEGER PRIMARY KEY, " +
					"bottleTypeLabel TEXT, " +
					"capacity NUMBER" +
				");";
			ddl.execute();

			var sql:SQLStatement = new SQLStatement();
			sql.sqlConnection = _connection;
			sql.text = "INSERT INTO bottleType ( bottleTypeLabel, capacity ) VALUES ( :bottleTypeLabel, :capacity )";

			sql.parameters[":bottleTypeLabel"] = "Stubby";
			sql.parameters[":capacity"] = "375";
			sql.execute();

			sql.parameters[":bottleTypeLabel"] = "Long Neck";
			sql.parameters[":capacity"] = "750";
			sql.execute();
		}
	}

	private function initBottleTable():void
	{
		if ( !this.doesTableExists( 'bottle' ) )
		{
			var ddl:SQLStatement = new SQLStatement();
			ddl.sqlConnection = _connection;
			ddl.text =
				"CREATE TABLE bottle (" +
					"bottleTypeSerial INTEGER, " +
					"brewSerial INTEGER, " +
					"count NUMBER, " +
					"used NUMBER" +
				");";
			ddl.execute();
		}
	}

	private function doesTableExists( tableName:String ):Boolean
	{
		try
		{
			var sql:SQLStatement = new SQLStatement();
			sql.sqlConnection = _connection;
			sql.text = "SELECT COUNT(*) FROM " + tableName + ";";
			sql.execute();
		}
		catch ( error:SQLError )
		{
			// http://help.adobe.com/en_US/as3/dev/WS112915e91f27785047d2b8021256b9bb733-8000.html
			if ( error.detailID == 2013 ) // (No such table)
			{
				return false;
			}
			else
			{
				throw new Error( "Error checking if table exists (" + error.detailID + "): " + error.details );
			}
		}
		return true;
	}

}

}
