package com.serwylo.brewdiary.data
{
import mx.collections.ArrayList;

public class Note
{

	public var noteSerial:Number;
	public var type:NoteType;
	public var message:String;
	public var createdDate:Date = new Date();

	public function Note()
	{
	}

	public function getIcon():Class
	{
		switch( this.type.noteTypeSerial )
		{
			case NoteType.ERROR:
				return NoteType.IMG_ERROR;
			break;

			case NoteType.IDEA:
				return NoteType.IMG_IDEA;
			break;

			case NoteType.NOTE:
			default:
				return NoteType.IMG_NOTE;
			break;
		}
	}

}

}
