var Util = {

	isArray: function( ary ){
		return Object.prototype.toString.call( ary ) === '[object Array]';
	},
	
	copy: function( _obj ){
		var obj = {};
		for ( var i in _obj ){
			if ( Util.isArray( _obj[i] ) ){
				obj[ i ] = _obj[ i ].slice( 0 );
			}else{
				obj[ i ] = _obj[ i ];	
			}
		}
		return obj;
	},
	
	images: {

	}
	
	getImage: function(   ){
		 
	},
	
	loadImg: function(   ){

		var root = 'images/', count = 0, num = 0;

		
	},
	
	audioObj: {},
	

	
}




