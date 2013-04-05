/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 8/01/11
 * Time: 8:19 PM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.services
{
import com.serwylo.brewdiary.data.Bottle;
import com.serwylo.brewdiary.data.BottleType;
import com.serwylo.brewdiary.data.Brew;

import flash.data.SQLResult;
import flash.data.SQLStatement;

public class BottleService extends SqlService
{

	public static function fillBottleWithBrew( brew:Brew, bottle:Bottle ):void
	{
		var query:String =
				"REPLACE INTO bottle " +
						"( bottleTypeSerial, brewSerial, count )" +
						"VALUES" +
						"( :bottleTypeSerial, :brewSerial, :count )";
		var params:Object = { bottleTypeSerial : bottle.bottleType.bottleTypeSerial, brewSerial : brew.brewSerial, count : bottle.count };
		SqlService.query( query, params );
	}

	private static var _bottleType:Array;

	public static function getBottleTypeList():Array
	{
		if ( _bottleType == null )
		{
			var query:String = "SELECT bottleTypeSerial, bottleTypeLabel, capacity FROM bottleType;";
			var sql:SQLStatement = SqlService.query( query );

			_bottleType = new Array();
			var result:SQLResult = sql.getResult();
			if ( result )
			{
				for each ( var b:Object in result.data )
				{
					var bottleType:BottleType = new BottleType();
					bottleType.bottleTypeLabel = b.bottleTypeLabel;
					bottleType.bottleTypeSerial = b.bottleTypeSerial;
					bottleType.capacity = b.capacity;
					_bottleType.push( bottleType );
				}
			}
		}
		return _bottleType;
	}

	/**
	 * Doesn't connect to the database, rather it will loop through the status list.
	 * This is because the bottleType list pretty much always exists, and it will be nice to have a reference
	 * to the same object everywhere rather than creating a new one each time.
	 * @param bottleTypeSerial
	 * @return
	 */
	public static function getBottleTypeBySerial( bottleTypeSerial:Number ):BottleType
	{
		for each ( var b:BottleType in getBottleTypeList() )
		{
			if ( b.bottleTypeSerial == bottleTypeSerial )
			{
				return b;
			}
		}
		return null;
	}

	public static function getBottlesByBrew( brew:Brew ):Array
	{
		var query:String = "SELECT bottleTypeSerial, count, used FROM bottle WHERE brewSerial = :brewSerial;";
		var params:Object = { brewSerial : brew.brewSerial };
		var sql:SQLStatement = SqlService.query( query, params );

		var bottleList:Array = new Array();
		var result:SQLResult = sql.getResult();
		if ( result )
		{
			for each ( var b:Object in result.data )
			{
				var bottle:Bottle = new Bottle();
				bottle.bottleType = BottleService.getBottleTypeBySerial( b.bottleTypeSerial );
				bottle.count = b.count;
				bottle.used = b.used;
				bottleList.push( bottle );
			}
		}

		return bottleList;
	}
}

}
