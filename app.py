# Copyright (c) 2025 - MSCS632 - Group 8 Spring 2025
# All rights reserved.
#
# This file is the property of MSCS632 - Group 8 Spring 2025.
# Unauthorized use, copying, or distribution is strictly prohibited.
# MSCS632 - Group 8 Spring 2025 reserves the right to take legal action
# against any person or entity who makes use of this file without the
# express written permission of MSCS632 - Group 8 Spring 2025.
# 

from flask import Flask, render_template, request, redirect, url_for
from datetime import datetime
from typing import List, Dict, Union
import os

app = Flask(__name__, template_folder='ui')

# Define a TypedDict for expense structure
from typing import TypedDict

class Expense(TypedDict):
    date: datetime
    amount: float
    category: str
    description: str

# Added some type safety for expenses
expenses: List[Expense] = []
categories: set[str] = {"Default Category"}

def add_expense(date_str: str, amount: str, category: str, description: str) -> None:
    """
    Adds a new expense to the in-memory list.
    Args:
        date_str: Date in YYYY-MM-DD format
        amount: String representation of a float number
        category: Category name
        description: Expense description
    """
    expense: Expense = {
        "date": datetime.strptime(date_str, "%Y-%m-%d"),
        "amount": float(amount),
        "category": category,
        "description": description
    }
    expenses.append(expense)

def delete_expense(index: int) -> None:
    """
    Deletes an expense from the in-memory list by index.
    Args:
        index: Index of the expense to delete
    """
    if 0 <= index < len(expenses):
        expenses.pop(index)

@app.route("/categories", methods=["GET", "POST"])
def manage_categories():
    if request.method == "POST":
        new_category = request.form["category"].strip()
        if new_category:  # Only add non-empty categories
            categories.add(new_category)
        return redirect(url_for("manage_categories"))
    return render_template("categories.html", categories=sorted(categories))

@app.route("/categories/delete/<category>")
def delete_category(category):
    if category in categories:
        categories.remove(category)
    return redirect(url_for("manage_categories"))

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Retrieve data from form submission
        date_str = request.form["date"]
        amount = request.form["amount"]
        category = request.form.get("category")
        description = request.form["description"]
        add_expense(date_str, amount, category, description)
        return redirect(url_for("index"))
    return render_template("index.html", 
                         expenses=expenses, 
                         categories=sorted(categories),
                         filter_category=request.args.get('category'))

@app.route("/filter", methods=["GET"])
def filter_expenses():
    """
    Filter expenses by optional start date, end date, category, and search term.
    """
    start_date = request.args.get("start_date")
    end_date = request.args.get("end_date")
    category_filter = request.args.get("category")
    search_term = request.args.get("search")

    filtered = expenses
    
    # Add date filtering
    if start_date:
        start_date = datetime.strptime(start_date, "%Y-%m-%d")
        filtered = [e for e in filtered if e["date"] >= start_date]
    if end_date:
        end_date = datetime.strptime(end_date, "%Y-%m-%d")
        filtered = [e for e in filtered if e["date"] <= end_date]
        
    if category_filter:
        filtered = [e for e in filtered if e["category"] == category_filter]
    if search_term:
        search_term = search_term.lower()
        filtered = [e for e in filtered if 
            search_term in e["description"].lower() or 
            search_term in e["category"].lower()]
    
    return render_template("index.html", 
                         expenses=filtered, 
                         categories=sorted(categories),
                         filter_category=category_filter)

@app.route("/summary", methods=["GET"])
def summary():
    """
    Calculate overall total and total per category, and organize expenses by category.
    """
    total_overall = sum(e["amount"] for e in expenses)
    summary_by_category = {}
    expenses_by_category = {}

    # Organize expenses by category and calculate totals
    for e in expenses:
        cat = e["category"]
        # Add to summary totals
        summary_by_category[cat] = summary_by_category.get(cat, 0) + e["amount"]
        
        # Organize expenses by category
        if cat not in expenses_by_category:
            expenses_by_category[cat] = []
        
        expenses_by_category[cat].append({
            'description': e["description"],
            'amount': e["amount"],
            'date': e["date"].strftime('%Y-%m-%d')
        })

    return render_template("summary.html", 
                         total=total_overall, 
                         summary=summary_by_category,
                         expenses=expenses_by_category)

@app.route("/delete/<int:index>")
def delete_expense_route(index):
    """
    Route to handle expense deletion
    """
    delete_expense(index)
    return redirect(url_for("index"))

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=int(os.environ.get('PORT', 8000)))