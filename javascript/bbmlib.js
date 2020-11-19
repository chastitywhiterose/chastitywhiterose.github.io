 /*javascript port of my polygon points algorithm*/
function get_polygon_points()
{
 var angle,x,y;
 var i=0;
 while(i<polygon_sides)
 {
  angle=2*Math.PI*i/polygon_sides+polygon_radians;
  polygon_xpoints[i]=polygon_cx+Math.sin(angle)*polygon_radius;
  polygon_ypoints[i]=polygon_cy-Math.cos(angle)*polygon_radius;
  i++;
 }
}


/*javascript port of my polygon draw function*/
function bbm_polygon()
{
 var i=0;
 get_polygon_points();
 ctx.beginPath();
 while(i<polygon_sides)
 {
  ctx.lineTo(polygon_xpoints[i],polygon_ypoints[i]);  
  i++;
 }
 ctx.closePath();
 ctx.fill();
}

/*javascript port of my star polygon draw function*/
function bbm_star()
{
 var i=0,i1=0;
 get_polygon_points();
 ctx.beginPath();
 while(i<polygon_sides)
 {
  i1=(i+polygon_step)%polygon_sides;
  ctx.lineTo(polygon_xpoints[i],polygon_ypoints[i]);
  ctx.lineTo(polygon_xpoints[i1],polygon_ypoints[i1]);
  ctx.lineTo(polygon_cx,polygon_cy);
  i+=1;
 }
 ctx.closePath();
 ctx.fill();
}

/*draw a circle*/
function bbm_circle()
{
 ctx.beginPath();
 ctx.arc(polygon_cx, polygon_cy, polygon_radius, 0, 2*Math.PI);
 ctx.closePath();
 ctx.fill();
}
