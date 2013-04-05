package com.serwylo.brewdiary.services
{
import com.serwylo.brewdiary.data.Brew;
import com.serwylo.brewdiary.data.Note;
import com.serwylo.brewdiary.data.NoteType;

import com.serwylo.brewdiary.data.SgReading;

import flash.data.SQLResult;
import flash.data.SQLStatement;
import flash.data.SQLStatement;

public class SgService extends SqlService
{

	private static var _noteTypeList:Array;

	public function SgService()
	{
	}

	public static function removeReading( reading:SgReading ):void
	{
		var query:String = "DELETE FROM sgReading WHERE sgReadingSerial = :sgReadingSerial";
		var params:Object = { sgReadingSerial : reading.sgReadingSerial };
		SqlService.query( query, params );
	}

	public static function saveReading( reading:SgReading, brew:Brew ):void
	{
		if ( reading.sgReadingSerial > 0 )
		{
			var query:String =
				"UPDATE sgReading " +
				"SET sgValue = :sgValue, " +
				"readingDate = :readingDate" +
				"WHERE sgReadingSerial = :sgReadingSerial";

			var params:Object = {
				sgReadingSerial : reading.sgReadingSerial,
				sgValue : reading.sgValue,
				readingDate : reading.readingDate
			};

			SqlService.query( query, params );
		}
		else
		{
			query =
				"INSERT INTO sgReading " +
				"(sgValue, readingDate, brewSerial) " +
				"VALUES " +
				"(:sgValue, :readingDate, :brewSerial)";

			params = {
				sgValue : reading.sgValue,
				readingDate : reading.readingDate,
				brewSerial : brew.brewSerial
			};

			SqlService.query( query, params );
			reading.sgReadingSerial = SqlService.lastInsertId();

		}
	}

	public static function getSgReadingsByBrew( brew:Brew ):Array
	{
		var query:String = "SELECT sgReadingSerial, sgValue, readingDate FROM sgReading WHERE brewSerial = :brewSerial;";
		var params:Object = { brewSerial : brew.brewSerial };
		var sql:SQLStatement = SqlService.query( query, params );

		var readingList:Array = new Array();
		var result:SQLResult = sql.getResult();
		for each ( var n:Object in result.data )
		{
			var sgReading:SgReading = new SgReading();
			sgReading.sgReadingSerial = n.sgReadingSerial;
			sgReading.sgValue = n.sgValue;
			sgReading.readingDate = n.readingDate;
			readingList.push( sgReading );
		}

		return readingList;
	}

}
}
