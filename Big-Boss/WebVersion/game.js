
var div1, div2, div3, loading, mode = 1, ready = false;

var Game = function(){

	var init = function( hash ){
		
		if ( hash ){
			mode = hash;
			return gameStart();
		}
		loading = document.createElement( 'img' );
		
		loading.src = 'images/g/loading.jpg';



		var wrap = document.body.appendChild( document.createElement( "div" ) )

		document.body.appendChild( loading );
		
		div = document.body.appendChild( document.createElement( 'div' ) );
		
		div.style.position = 'absolute';
		
		div.style.top = '200px';
		
		div.style.left = '450px';

		div.style.fontSize = '45px'

		div.style.color = '#fff';
	
		div.style.width = '500px';
		
		div.style.height = '100px'

		div.innerHTML = 'loading...';

		div2 = document.body.appendChild( document.createElement( 'div' ) );
				
		div2.style.position = 'absolute';
		
		div2.style.top = '300px';
		
		div2.style.left = '450px';

		div2.style.fontSize = '35px'
		
		div2.style.color = '#fff';
		
		div2.style.width = '500px';
		
		div2.style.height = '500px';
		
	


		document.body.style.position = 'relative'
		document.body.style.left = ( document.body.offsetWidth  - 900 ) / 2 + 'px'
		document.body.style.top = '20px'
		document.body.style.overflow = 'hidden';
		document.body.style.backgroundColor = '#A0AAB2';


		
		Util.loadImg( function(){	
			
			Util.loadAudio( function(){
			
				gameStart();


				var div = document.createElement( 'div' );
				div.style.width = '720px';
				div.style.height = '32px';
				div.style.top = '550px';
				div.style.left = '96px';
				div.style.position = 'absolute';
				div.style.zIndex = 9999;
				document.body.appendChild( div );

	
			})
		
		});


	}

	return {
		init: init
	}


}();

