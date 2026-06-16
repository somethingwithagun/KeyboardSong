<!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Results</title>
  </head>
  <body>



    <?php
      $scores = file('./scores.txt');
      foreach($scores as $cur) {
        echo $cur;
        echo "<br>";
      }
    ?>

  </body>
</html>