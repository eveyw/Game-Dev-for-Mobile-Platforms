var Event = function(){

	var obj = {};

	var listen = function( key, eventfn, type ){
		obj[ key ] || ( obj[ key ] = Interfaces.Queue() );
		if ( type === 1 ){
			obj[ key ].clean();
		}
		obj[ key ].add( eventfn );		
	}

	var removeListen = function( key ){
		obj[ key ] && obj[ key ].clean();
	}

	var fireEvent = function( key ){
		obj[ key ] && obj[ key ].fireEach.apply( this, Array.prototype.slice.call( arguments, 1 ) );
	}
	
	return {
		listen: listen,
		removeListen: removeListen,
		fireEvent: fireEvent	
	}

	
}





var Interfaces = {
	
	Lock: function(){

		var flag = false, timer, lock_delay, lock_level = 0;

		var lock = function( type, delay ){
		//	lock_delay = delay || 0;
		//alert ( type )
			lock_level = type;
			locked = type;
			//if ( type > _type || type === 0 ){
				//return unlock( type );
		//	}
		}

		var unlock = function( _type ){
			//if ( !locked() ) return;
			//timer && clearTimeout( timer );
			//timer = setTimeout( function(){
				//	_type = type || 0;
			//}, lock_delay || 0 );
		}

		var locked = function(){
			return lock_level > 0;
		}

		var getLevel = function(){
			return lock_level;
		}

		return {
			lock: lock,
			locked: locked,
			unlock: unlock,
			getLevel: getLevel
		}
	},

	Queue: function(){

		var stack = [];

		var add = function( obj ){
			if ( Util.isArray( obj ) ){
				return stack = stack.concat( obj );
			}
			stack.push( obj );
			return stack;
		}

		var unshift = function( obj ){
			stack.unshift( obj );	
		}
	
		var dequeue = function(){
			return stack.length && stack.shift();
		}

		var clean = function(){
			return stack.length = 0;	
		}
		
		var isEmpty = function(){
			return stack.length === 0;	
		}

		var fireEach = function(){
			for ( var i = 0, c; c = stack[i++]; ){
				c.apply( this, Array.prototype.slice.call( arguments, 0 ) );
			}
		}

		var get = function(){
			return stack;	
		}

		var last = function(){
			return stack.length && 	stack[ stack.length - 1 ];
		}
		
		return {
			add: add,
			unshift: unshift,
			dequeue: dequeue,
			clean: clean,
			isEmpty: isEmpty,
			fireEach: fireEach,
			get: get,
			last: last
		}

	},


	StatusManage: function(){

		

	}
}


