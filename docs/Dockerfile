FROM python:3.11-slim

WORKDIR /docs

# Install Sphinx and dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy documentation source
COPY source ./source
COPY Makefile .

# Build HTML documentation
CMD ["make", "html"]
