package com.serwylo.brewdiary.services
{
import com.serwylo.brewdiary.data.Brew;
import com.serwylo.brewdiary.data.Note;
import com.serwylo.brewdiary.data.NoteType;

import flash.data.SQLResult;
import flash.data.SQLStatement;
import flash.data.SQLStatement;

public class NoteService extends SqlService
{

	private static var _noteTypeList:Array;

	public function NoteService()
	{
	}

	public static function getNoteTypeList():Array
	{
		if ( _noteTypeList == null )
		{
			var sql:SQLStatement = query( "SELECT noteTypeSerial, noteTypeLabel FROM noteType;" );
			_noteTypeList = new Array();
			var result:SQLResult = sql.getResult();
			for each ( var nt:Object in result.data )
			{
				var noteType:NoteType = new NoteType();
				noteType.noteTypeLabel = nt.noteTypeLabel;
				noteType.noteTypeSerial = nt.noteTypeSerial;
				_noteTypeList.push( noteType );
			}
		}

		return _noteTypeList;
	}

	public static function saveNote( note:Note, brew:Brew ):void
	{
		if ( note.noteSerial > 0 )
		{
			var query:String =
				"UPDATE note " +
				"SET noteTypeSerial = :noteTypeSerial, " +
				"message = :message" +
				"WHERE noteSerial = :noteSerial";

			var params:Object = {
				noteTypeSerial : note.type.noteTypeSerial,
				message : note.message,
				noteSerial : note.noteSerial
			};

			SqlService.query( query, params );
		}
		else
		{
			query =
				"INSERT INTO note " +
				"(noteTypeSerial, message, createdDate, brewSerial) " +
				"VALUES " +
				"(:noteTypeSerial, :message, :createdDate, :brewSerial)";

			params = {
				noteTypeSerial : note.type.noteTypeSerial,
				message : note.message,
				createdDate : note.createdDate,
				brewSerial : brew.brewSerial
			};

			SqlService.query( query, params );
			note.noteSerial = SqlService.lastInsertId();

		}
	}

	public static function getNoteListByBrew( brew:Brew ):Array
	{
		var query:String = "SELECT noteSerial, message, noteTypeSerial, createdDate FROM note WHERE brewSerial = :brewSerial;";
		var params:Object = { brewSerial : brew.brewSerial };
		var sql:SQLStatement = SqlService.query( query, params );

		var noteList:Array = new Array();
		var result:SQLResult = sql.getResult();
		for each ( var n:Object in result.data )
		{
			var note:Note = new Note();
			note.noteSerial = n.noteSerial;
			note.message = n.message;
			note.type = NoteService.getNoteTypeBySerial( n.noteTypeSerial );
			noteList.push( note );
		}

		return noteList;
	}

	public static function getNoteTypeBySerial( noteTypeSerial:Number ):NoteType
	{
		for each ( var noteType:NoteType in NoteService.getNoteTypeList() )
		{
			if ( noteType.noteTypeSerial == noteTypeSerial )
			{
				return noteType;
			}
		}
		return null;
	}
}
}
