<?php
  // Check if the POST data contains the 'score' parameter
  if(isset($_POST['score']) and isset($_POST['username']) ) {
    $score = $_POST['score'];
    $username = $_POST['username'];

  // Open the file to store the scores (create it if it doesn't exist)
  $file = fopen("scores.txt", "a");
  
  // Write the score to the file
  fwrite($file, $username . ' ');
  fwrite($file, $score . PHP_EOL);
  // Close the file
  fclose($file);

  echo "Score posted successfully!\n";
}
?>