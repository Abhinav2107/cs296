import sys
import re

# reads a file an returns it as a list of words
def read_file(path):
	#print(path, " loaded.")
	f = open(path, 'r')
	file = f.read().split(' ')
	file = [x.replace('\n', '') for x in file if x]
	file = [x.replace('.', '') for x in file if x]
	file = [x.replace(',', '') for x in file if x]
	file = [x.replace('(', '') for x in file if x]
	file = [x.replace(')', '') for x in file if x]
	file = [x.replace('!', '') for x in file if x]
	file = [x.replace('?', '') for x in file if x]
	file = [x.replace(':', '') for x in file if x]
	file = [x.replace(';', '') for x in file if x]
	file = [re.sub('[1234567890]', '', x) for x in file if x]

#	file = [x for x in file if file.count(x) >= 2] toooo timeconsuming
	file = [x.lower() for x in file if x]
	return file

def normalize_score(score, keywords):
	return score / len(keywords)

def get_score(file, keywords):
	ret = dict()
	for keyword in keywords:
		ret[keyword] = file.count(keyword)
	score = sum(ret.values())
	return normalize_score(score, keywords)

def score_genres(file, genres):
	ret = dict()
	for genre, keywords in genres.items():
		ret[genre] = get_score(file, keywords)
	return ret.copy()

def select_genres(scores):
	ret = dict()
	avg_score = max(scores.values())**.5
	for genre, score in scores.items():
		if score > avg_score:
			ret[genre] = score

	return ret

def categorize_movie(path, possible_genres):
	file = read_file(path)
	scores = score_genres(file, possible_genres)
	ret = select_genres(scores)
	ranking = sorted(ret.items(), key=lambda x:x[1])
	ranking.reverse()
	return [row[0] for row in ranking]


# fixme
def detect_drama(genres):
	if len(genres) > 3:
		return {'Drama': len(genres)}
	else:
		return genres
	"""
	scores = list(genres.values())
	avg = sum(scores)/len(scores)
	threshold = avg * .4
	maxDelta = max([j-i for i, j in zip(scores[:-1], scores[1:])])
	if maxDelta > threshold and len(genres) > 2:
		return genres
	else:
		return {'Drama': maxDelta}
	"""

def run(movies, genres):
	for mov in movies:
		spaces = ''
		for x in range(50-len(mov)):
			spaces = spaces + ' ' 
		print(mov+':'+spaces+str(categorize_movie(mov, genres)))

MOVIES = [
	'data/horror/SAW.txt',
	'data/horror/THE_SILENCE_OF _THE_LAMBS.txt',
	'data/horror/FRANKENSTEIN.txt',
	'data/comedy/BAD_BOYS.txt',
	'data/comedy/THE_HANGOVER.txt',
	'data/comedy/DESPICABLE_ME_2.txt',
	'data/romance/TITANIC.txt',
	'data/romance/SEX_AND_THE_CITY.txt',
	'data/romance/TWILIGHT.txt',
	'data/sci-fi/I_ROBOT.txt',
	'data/sci-fi/ARMAGEDDON.txt',
	'data/sci-fi/ET.txt',
	'data/fantasy/EXCALIBUR.txt',
	'data/fantasy/LORD_OF_THE_RINGS.txt',
	'data/fantasy/THE_CHRONICLES_OF_NARNIA.txt',
	'data/action/48HOURS.txt',
	'data/action/AVP.txt',
	'data/action/DIE_HARD.txt',
	'data/action/BATMAN.txt',
	'data/drama/FIGHT_CLUB.txt',
	'data/drama/AMERICAN_BEAUTY.txt',
	'data/drama/DEAD_POETS_SOCIETY.txt',
	'data/drama/LES_MISERABLES.txt',
]

GENRES = dict()
GENRES['ACTION'] = ['automatic', 'gunfire',  'shooting', 'headset', 'weapon', 'elbow', 'offers', 'radio', 'crash', 'races', 'drifts', 'struggle', 'deadly', 'armed', 'report', 'halfway', 'panic', 'buttons', 'dives', 'trained', 'cigarette']
GENRES['COMEDY'] = ['sunglasses', 'butt', 'funny', 'embarrassed', 'apologize', 'wedding', 'birthday', 'fun', 'blonde', 'games', 'bar', 'champagne', 'beer', 'marry', 'nervous', 'breakfast', 'cute', 'club', 'cool', 'cash', 'drinks']
GENRES['DRAMA'] = ['military', 'war', 'behavior', 'jesus', 'weak', 'strike', 'proud', 'dramatic', 'ashtray', 'shaving', 'project', 'uniform', 'trouble', 'doorbell', 'accept']
GENRES['ROMANCE'] = ['boyfriend', 'firmly', 'closet', 'salad', 'pleased', 'stunning', 'mirrored', 'barefoot', 'hoping', 'effort','amazing', 'amazed', 'diamond', 'gorgeous', 'private', 'romantic', 'beauty', 'beautiful', 'sinks', 'gym']
GENRES['SCI-FI'] = ['robot', 'spaceship', 'machine',  'star', 'orbit', 'space', 'spaceshuttle', 'stars', 'earth', 'astronaute', 'structure', 'response', 'development', 'failure', 'blockingpanel', 'battery', 'electrical', 'device']
GENRES['HORROR'] = ['scarry', 'silently', 'blood', 'darkness', 'violent', 'violently', 'scars', 'dies', 'died', 'bloody', 'kills', 'kill', 'shot', 'pain', 'murder', 'flickering', 'unaware', 'creature', 'dispair', 'chamber', 'looming', 'fault', 'blade']
GENRES['FANTASY'] = ['dwarf', 'elfs', 'orks', 'sword', 'axe', 'battle', 'wizard', 'spell', 'king', 'queen', 'horse', 'armor', 'north', 'forgotten', 'mighty', 'weaopns']


run(MOVIES, GENRES)

FILES = [read_file(movie) for movie in MOVIES]
SETS = [set(file) for file in FILES]

horrorSet = (SETS[0] & SETS[1] & SETS[2])
comedySet = (SETS[3] & SETS[4] & SETS[5])
romanceSet = (SETS[6] & SETS[7] & SETS[8])
scifiSet = (SETS[9] & SETS[10] & SETS[11])
fantasySet = (SETS[12] & SETS[13] & SETS[14])
actionSet = (SETS[15] & SETS[16] & SETS[17] & SETS[18])
dramaSet = (SETS[19] & SETS[20] & SETS[21] & SETS[22])

commonWords = set()
commonWords = set([commonWords.update(SETS[i]) for i in range(len(SETS))])

romance_words = romanceSet - (horrorSet | comedySet | scifiSet | fantasySet | actionSet | dramaSet)
comedy_words = comedySet - (horrorSet | romanceSet | scifiSet | fantasySet | actionSet | dramaSet)
fantasy_words = fantasySet - (horrorSet | romanceSet | scifiSet | actionSet | dramaSet | comedySet)
scifi_words = scifiSet - (horrorSet | romanceSet | fantasySet | actionSet | dramaSet | comedySet)
action_words = actionSet - (horrorSet | romanceSet | scifiSet | fantasySet | dramaSet | comedySet)
drama_words = dramaSet - (horrorSet | romanceSet | scifiSet | fantasySet | actionSet | comedySet)
horror_words = horrorSet - (dramaSet | romanceSet | scifiSet | fantasySet | actionSet | comedySet)

#print(romance_words)
#print(comedy_words)
#print(fantasy_words)
#print(horror_words)
#print(drama_words)
#print(scifi_words)
#print(action_words)