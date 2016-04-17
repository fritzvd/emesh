var gulp = require('gulp')
var child_process = require('child_process')
var nestmx = require('nes-tmx')

var paths = {
  graphics: 'gfx/tiles.png',
  tilemap: 'gfx/tilemap.tmx',
  src: 'src/**/*'
}

gulp.task('spriteBuild', createTiles)
gulp.task('nameTables', nameTables)
gulp.task('build', make)

gulp.task('dev', function() {
  gulp.watch(paths.graphics, ['spriteBuild']);
  gulp.watch(paths.tilemap, ['nameTables']);
  gulp.watch(paths.src, ['build']);
})

function createTiles () {
  var pilbmp2nes = child_process.spawn(
    './tools/pilbmp2nes.py', ['-i', 'gfx/tiles.png', '-o', 'src/tiles.chr'])
    pilbmp2nes.stdout.on('data', function (d) {
      console.log('' + d)
    })
    pilbmp2nes.stderr.on('data', function (d) {
      console.error('' + d)
    })
  return pilbmp2nes
}

function nameTables () {
  nestmx('gfx/tilemap.tmx', 'src/collision.h', 'src/nametable.h')
}

function make () {
  var make = child_process.spawn('make', {cwd: 'src'})

  make.stdout.on('data', function (d) {
    console.log('' + d)
  })
  make.stderr.on('data', function (d) {
    console.error('' + d)
  })
  return make;
}

gulp.task('default', ['build'])
