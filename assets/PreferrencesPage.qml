import bb.cascades 1.2

Page {

    property alias defaultUser: defaultUserText.text
     
	Container {
	
	    layout: StackLayout {
	        //orientation: LayoutOrientation.LeftToRight
	    }
	
	    // selection form
	    TextField {
	        id: defaultUserText
	        hintText: "User Name. Example: 'from-where-i-stand'"
	    }
	}
}

