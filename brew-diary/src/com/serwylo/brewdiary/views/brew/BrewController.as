package com.serwylo.brewdiary.views.brew
{
import com.ivt.flex.utils.HelperDate;
import com.serwylo.brewdiary.core.Controller;

import com.serwylo.brewdiary.core.View;
import com.serwylo.brewdiary.data.Bottle;
import com.serwylo.brewdiary.data.Brew;

import com.serwylo.brewdiary.data.Note;
import com.serwylo.brewdiary.data.Review;
import com.serwylo.brewdiary.data.SgReading;
import com.serwylo.brewdiary.data.Status;
import com.serwylo.brewdiary.events.DataChangeEvent;
import com.serwylo.brewdiary.forms.BottledForm;
import com.serwylo.brewdiary.forms.NoteForm;

import com.serwylo.brewdiary.forms.ReviewForm;

import com.serwylo.brewdiary.forms.SgForm;
import com.serwylo.brewdiary.services.BottleService;
import com.serwylo.brewdiary.services.BrewService;
import com.serwylo.brewdiary.services.NoteService;
import com.serwylo.brewdiary.services.SgService;
import com.serwylo.brewdiary.services.StatusService;

import flash.events.Event;

import mx.collections.ArrayCollection;
import mx.controls.Alert;
import mx.events.CloseEvent;

public class BrewController extends Controller
{

	private var _view:BrewView;
	private var _model:BrewModel;

	public function BrewController()
	{
		this._view = new BrewView();
		this._view.addEventListener( BrewView.ADD_NOTE, this.onAddNote );
		this._view.addEventListener( DataChangeEvent.DATA_CHANGE, this.onDataChange );
		this._view.addEventListener( BrewView.EDIT_BOTTLES, this.onEditBottles );
		this._view.addEventListener( BrewView.ADD_SG_READING, this.onAddSgReading );
		this._view.addEventListener( BrewView.DELETE_SG_READING, this.onDeleteSgReading );

		this._model = new BrewModel();
		this._model.statusList = new ArrayCollection( StatusService.getStatusList() );
		this._view.setup( this._model );
	}

	public override function get view():View { return this._view; }

	public function set batch( brew:Brew ):void
	{
		this._model.brew = brew;
		if ( this._model.brew.notes == null )
		{
			this._model.brew.notes = new ArrayCollection( NoteService.getNoteListByBrew( brew ) );
		}
		if ( this._model.brew.bottles == null )
		{
			this._model.brew.bottles = new ArrayCollection( BottleService.getBottlesByBrew( brew ) );
		}
		if ( this._model.brew.sgReadings == null )
		{
			this._model.brew.sgReadings = new ArrayCollection( SgService.getSgReadingsByBrew( brew ) );
		}
	}

	private function onAddNote( event:Event ):void
	{
		var form:NoteForm = NoteForm.show();
		form.addEventListener( Event.SELECT, this.onNoteAdded );
	}

	private function onNoteAdded( event:Event ):void
	{
		var form:NoteForm = event.target as NoteForm;
		var note:Note = new Note();
		note.type = form.typeValue;
		note.message = form.messageValue;
		this._model.brew.notes.addItem( note );
		NoteService.saveNote( note, this._model.brew );
	}

	private function onDataChange( event:DataChangeEvent ):void
	{
		switch( event.kind )
		{
			case "status":
				this.changeStatus( event.data as Status );
			break;
		}
	}

	private function changeStatus( status:Status ):void
	{
		this._model.brew.status = status;
		BrewService.saveBrewService( this._model.brew );
		switch( status.statusSerial )
		{
			case Status.STATUS_INITIAL_FERMENTATION:
				// Cool...
			break;

			case Status.STATUS_BOTTLE_FERMENTATION:
				// How many bottles are there?
				var bottledForm:BottledForm = BottledForm.showAfterBottling();
				bottledForm.addEventListener( Event.SELECT, this.onBottledFormSubmitted );
			break;

			case Status.STATUS_BOTTLED_AGEING:
				// Create a note at this time...
				bottledForm = BottledForm.showAfterBottleFermentation();
				bottledForm.addEventListener( Event.SELECT, this.onBottledFormSubmitted );
			break;

			case Status.STATUS_READY:
				// Perhaps you would like to taste one and provide a review?
				var reviewForm:ReviewForm = ReviewForm.show();
				reviewForm.addEventListener( Event.SELECT, this.onReviewFormSubmitted );
			break;

			case Status.STATUS_EMPTY:
				// Make sure there is a rating...
			break;
		}
	}

	private function onBottledFormSubmitted( event:Event ):void
	{
		var form:BottledForm = event.target as BottledForm;
		var bottles:Array = form.bottleValues;
		for each ( var bottle:Bottle in form.bottleValues )
		{
			BottleService.fillBottleWithBrew( this._model.brew, bottle );
		}
	}

	private function onReviewFormSubmitted( event:Event ):void
	{
		var form:ReviewForm = event.target as ReviewForm;
		
		var review:Review = new Review();
		review.shortDescription = form.shortReviewValue;
		review.longDescription = form.longReviewValue;
		review.rating = form.ratingValue;

		this._model.brew.reviews.addItem( review );
	}

	private function onEditBottles( event:Event ):void
	{
		var form:BottledForm = BottledForm.showNormal( this._model.brew.bottles );
		form.addEventListener( Event.SELECT, this.onBottledFormSubmitted );
	}

	private function onDeleteSgReading( event:Event ):void
	{
		var reading:SgReading = this._view.selectedReading;
		Alert.show( "Are you sure you want to remove this reading?\n\n  " + reading.sgValue.toFixed( 4 ) + "\n\n  " + HelperDate.formatDateTime( reading.readingDate ), "Delete Reading?", Alert.YES | Alert.NO, null, this.onConfirmDeleteReading );
	}

	private function onConfirmDeleteReading( result:CloseEvent ):void
	{
		var reading:SgReading = this._view.selectedReading;
		if ( result.detail == Alert.YES )
		{
			SgService.removeReading( reading );
			var i:Number = this._model.brew.sgReadings.getItemIndex( reading );
			if ( i != -1 )
			{
				this._model.brew.sgReadings.removeItemAt( i );
			}
		}
	}

	private function onAddSgReading( event:Event ):void
	{
		var form:SgForm = SgForm.show();
		form.addEventListener( Event.SELECT, this.onSgFormSubmitted );
	}

	private function onSgFormSubmitted( event:Event ):void
	{
		var form:SgForm = event.target as SgForm;

		var sgReading:SgReading = new SgReading();
		sgReading.readingDate = form.dateValue;
		sgReading.sgValue = form.sgValue;

		SgService.saveReading( sgReading, this._model.brew );

		this._model.brew.sgReadings.addItem( sgReading );
	}
}

}
