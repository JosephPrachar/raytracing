def AT3DToPointVertex():
  f = open('mesh.xyz', 'r')
  points = open('meshPoints.in', 'w')
  verticies = open('meshVerticies.in', 'w')

  f.readline()
  rows = int(f.readline().split()[1])
  cols = int(f.readline().split()[1])
  f.readline()
  f.readline()

  for y in range(0, rows - 1):
    for x in range(0, cols - 1):
      verticies.write(str(x + (y*cols)) + ' ' + str(x + (y*cols)+1) + ' ' + str(x + (y*cols) + cols) + '\n')

  
  for y in range(1, rows):
    for x in range(0, cols - 1):
      verticies.write(str(x + (y*cols)) + ' ' + str(x + (y*cols) - cols + 1) + ' ' + str(x + (y*cols) + 1) + '\n')

  for line in f:
    points.write(line)

AT3DToPointVertex();
