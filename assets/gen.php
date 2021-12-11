
<?php
list($width, $height, $type, $attr) = getimagesize("base.png");
$img = imagecreatefrompng("base.png");

for($line = 0; $line < $height; $line+=50){

for($col = 0; $col < $width; $col++){
$colorIndex = imagecolorat($img, $col,$line);
$colorInfo = imagecolorsforindex($img, $colorIndex);
//echo $colorInfo["alpha"] ." "; 
//echo "{".$col.",".$line."},";
if($colorInfo["alpha"] == 0){
echo "{".$col.",".$line."},";

break;
}



}

}


for($line = 0; $line < $height; $line+=50){

for($col = $width - 1; $col >=0; --$col){
$colorIndex = imagecolorat($img, $col,$line);
$colorInfo = imagecolorsforindex($img, $colorIndex);
//echo $colorInfo["alpha"] ." "; 
//echo "{".$col.",".$line."},";
if($colorInfo["alpha"] == 0){
echo "{".$col.",".$line."},";

break;
}



}

}

echo PHP_EOL;
echo $width." ".$height;
?>

