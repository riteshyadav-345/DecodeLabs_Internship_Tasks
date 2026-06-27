ITEMS = [
    {
        "name": "Inception",
        "category": "Movie",
        "tags": {"sci-fi", "thriller", "mystery", "action", "mind-bending"},
    },
    {
        "name": "Interstellar",
        "category": "Movie",
        "tags": {"sci-fi", "space", "drama", "adventure", "emotional"},
    },
    {
        "name": "The Social Network",
        "category": "Movie",
        "tags": {"technology", "business", "drama", "startup"},
    },
    {
        "name": "Atomic Habits",
        "category": "Book",
        "tags": {"self-improvement", "productivity", "habits", "motivation"},
    },
    {
        "name": "Rich Dad Poor Dad",
        "category": "Book",
        "tags": {"finance", "money", "business", "self-improvement"},
    },
    {
        "name": "Python Crash Course",
        "category": "Book",
        "tags": {"programming", "python", "technology", "learning"},
    },
    {
        "name": "Stardew Valley",
        "category": "Game",
        "tags": {"relaxing", "farming", "simulation", "creative"},
    },
    {
        "name": "Minecraft",
        "category": "Game",
        "tags": {"creative", "adventure", "survival", "building"},
    },
    {
        "name": "Portal 2",
        "category": "Game",
        "tags": {"puzzle", "sci-fi", "comedy", "logic"},
    },
    {
        "name": "Kurzgesagt",
        "category": "YouTube Channel",
        "tags": {"science", "space", "learning", "animation"},
    },
]


def show_available_interests():
    all_tags = sorted({tag for item in ITEMS for tag in item["tags"]})

    print("Available interests:")
    for index, tag in enumerate(all_tags, start=1):
        print(f"{index:2}. {tag}")
    print()


def get_user_preferences():
    user_input = input(
        "Enter your interests separated by commas "
        "(example: sci-fi, space, adventure): "
    )

    preferences = {
        interest.strip().lower()
        for interest in user_input.split(",")
        if interest.strip()
    }

    return preferences


def calculate_similarity(preferences, item_tags):
    matching_tags = preferences.intersection(item_tags)
    total_unique_tags = preferences.union(item_tags)

    if not total_unique_tags:
        return 0, matching_tags

    score = len(matching_tags) / len(total_unique_tags)
    return score, matching_tags


def recommend_items(preferences, limit=3):
    recommendations = []

    for item in ITEMS:
        score, matching_tags = calculate_similarity(preferences, item["tags"])

        if score > 0:
            recommendations.append(
                {
                    "name": item["name"],
                    "category": item["category"],
                    "score": score,
                    "matching_tags": matching_tags,
                }
            )

    recommendations.sort(key=lambda item: item["score"], reverse=True)
    return recommendations[:limit]


def display_recommendations(recommendations):
    if not recommendations:
        print("\nNo strong matches found.")
        print("Try interests like sci-fi, space, learning, finance, or adventure.")
        return

    print("\nRecommended items:")
    for index, item in enumerate(recommendations, start=1):
        matched = ", ".join(sorted(item["matching_tags"]))
        percentage = item["score"] * 100

        print(f"{index}. {item['name']} ({item['category']})")
        print(f"   Match score: {percentage:.1f}%")
        print(f"   Because you like: {matched}")


def main():
    print("Simple Recommendation System")
    print("----------------------------")
    show_available_interests()

    preferences = get_user_preferences()

    if not preferences:
        print("\nPlease enter at least one interest.")
        return

    recommendations = recommend_items(preferences)
    display_recommendations(recommendations)


if __name__ == "__main__":
    main()
