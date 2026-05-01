# Use official GCC image (has compiler pre-installed)
FROM gcc:latest

# Set working directory inside the container
WORKDIR /app

# Copy everything from the project into container
COPY . .

# Compile the C++ program
RUN g++ -std=c++17 -O2 src/main.cpp -o sentinel

# Default command to run when container starts
# This is equivalent to ./sentinel (the program entry point)
CMD ["./sentinel"]